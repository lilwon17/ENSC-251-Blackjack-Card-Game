// ============================================================================
//  Blackjack - graphical version
//  ENSC 251 - group project
//
//  Reuses the Card, Deck and Hand classes from the original console project.
//  The turn logic and all the drawing live here, inside a state machine that
//  runs in raylib's game loop.
//
//  Controls:  H / click  -> Hit        S / click -> Stand
//             Y          -> Play again N / ESC   -> Quit
// ============================================================================

#include "raylib.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>

#include "Card.hpp"
#include "Deck.hpp"
#include "Hand.hpp"

// ----------------------------------------------------------------------------
//  Presentation constants
// ----------------------------------------------------------------------------
static const int   SCREEN_W = 1100;
static const int   SCREEN_H = 720;

static const float CARD_W = 100.0f;
static const float CARD_H = 145.0f;
static const float CARD_GAP = 26.0f;   // horizontal gap between cards

static const float DEALER_ROW_Y = 105.0f;   // top row    -> dealer
static const float PLAYER_ROW_Y = 415.0f;   // bottom row -> player

static const Vector2 DECK_POS = { 940.0f, 40.0f };  // cards fly out from here

static const float DEAL_SPEED = 2.6f;   // 1.0 / seconds the flight takes
static const float DEAL_GAP = 0.28f;  // delay between one card and the next
static const float FLIP_SPEED = 3.0f;   // flip speed of the hole card

// Palette
static const Color FELT_DARK = { 11,  61,  38, 255 };
static const Color FELT_LIGHT = { 21,  94,  60, 255 };
static const Color CARD_FACE = { 250, 250, 245, 255 };
static const Color CARD_BACK = { 24,  52, 110, 255 };
static const Color CARD_BACK2 = { 42,  84, 165, 255 };
static const Color RED_SUIT = { 200, 32,  46, 255 };
static const Color BLACK_SUIT = { 28,  28,  32, 255 };
static const Color GOLD_TRIM = { 226, 183,  82, 255 };
static const Color WIN_GREEN = { 46, 190, 106, 255 };
static const Color LOSE_RED = { 224, 62,  62, 255 };

// ----------------------------------------------------------------------------
//  Game states
// ----------------------------------------------------------------------------
enum GameState {
    STATE_DEALING,       // dealing the 4 opening cards
    STATE_PLAYER_TURN,   // player decides to hit or stand
    STATE_PLAYER_DRAW,   // the card just requested is flying in
    STATE_DEALER_FLIP,   // the dealer's hole card flips over
    STATE_DEALER_TURN,   // dealer hits until 17
    STATE_ROUND_OVER     // result + play again screen
};

enum RoundResult { RESULT_NONE, RESULT_WIN, RESULT_LOSE, RESULT_PUSH };

// A card on screen: the card itself plus its animation state.
struct CardView {
    Card  card;
    float t;         // 0 = still on the deck, 1 = arrived at its slot
    float delay;     // seconds left before it starts flying
    bool  faceDown;  // true = draw the back of the card
    float flip;      // 0 = face down, 1 = face up (dealer's hole card)

    CardView(Card c, float d, bool down)
        : card(c), t(0.0f), delay(d), faceDown(down), flip(down ? 0.0f : 1.0f) {
    }

    bool landed() const { return t >= 1.0f; }
};

// ----------------------------------------------------------------------------
//  Drawing helpers
// ----------------------------------------------------------------------------

// raylib culls triangles with the "wrong" winding order. Drawing it both ways
// guarantees it always shows up without having to guess the correct order.
static void DrawTri2(Vector2 a, Vector2 b, Vector2 c, Color col) {
    DrawTriangle(a, b, c, col);
    DrawTriangle(c, b, a, col);
}

// Ease-out cubic: starts fast and slows down as it lands.
static float EaseOut(float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    float inv = 1.0f - t;
    return 1.0f - inv * inv * inv;
}

static const char* RankText(int rank) {
    switch (rank) {
    case 1:  return "A";
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    default: break;
    }
    static char buf[4];
    snprintf(buf, sizeof(buf), "%d", rank);
    return buf;
}

// The 4 suits drawn with primitives (no image assets or special fonts needed).
static void DrawSuit(int suit, float cx, float cy, float r) {
    Color col = (suit == 1 || suit == 2) ? RED_SUIT : BLACK_SUIT;

    if (suit == 2) {                 // Hearts
        DrawCircle((int)(cx - r * 0.45f), (int)(cy - r * 0.28f), r * 0.52f, col);
        DrawCircle((int)(cx + r * 0.45f), (int)(cy - r * 0.28f), r * 0.52f, col);
        DrawTri2({ cx - r * 0.95f, cy - r * 0.20f },
            { cx + r * 0.95f, cy - r * 0.20f },
            { cx,             cy + r * 1.05f }, col);
    }
    else if (suit == 1) {            // Diamonds
        DrawPoly({ cx, cy }, 4, r * 1.05f, 0.0f, col);
    }
    else if (suit == 3) {            // Spades
        DrawCircle((int)(cx - r * 0.45f), (int)(cy + r * 0.18f), r * 0.52f, col);
        DrawCircle((int)(cx + r * 0.45f), (int)(cy + r * 0.18f), r * 0.52f, col);
        DrawTri2({ cx - r * 0.95f, cy + r * 0.24f },
            { cx + r * 0.95f, cy + r * 0.24f },
            { cx,             cy - r * 1.05f }, col);
        DrawTri2({ cx - r * 0.42f, cy + r * 1.05f },
            { cx + r * 0.42f, cy + r * 1.05f },
            { cx,             cy + r * 0.30f }, col);
    }
    else {                           // Clubs
        DrawCircle((int)cx, (int)(cy - r * 0.48f), r * 0.44f, col);
        DrawCircle((int)(cx - r * 0.52f), (int)(cy + r * 0.20f), r * 0.44f, col);
        DrawCircle((int)(cx + r * 0.52f), (int)(cy + r * 0.20f), r * 0.44f, col);
        DrawTri2({ cx - r * 0.38f, cy + r * 1.05f },
            { cx + r * 0.38f, cy + r * 1.05f },
            { cx,             cy + r * 0.15f }, col);
    }
}

static void DrawCardBack(float x, float y, float w, float h) {
    Rectangle rec = { x, y, w, h };
    DrawRectangleRounded(rec, 0.10f, 8, CARD_BACK);
    Rectangle inner = { x + 7, y + 7, w - 14, h - 14 };
    DrawRectangleRounded(inner, 0.10f, 8, CARD_BACK2);

    // decorative diamond pattern
    for (int row = 0; row < 5; row++) {
        for (int colI = 0; colI < 3; colI++) {
            float px = x + 22 + colI * (w - 44) / 2.0f;
            float py = y + 24 + row * (h - 48) / 4.0f;
            DrawPoly({ px, py }, 4, 7.0f, 0.0f, CARD_BACK);
        }
    }
}

static void DrawCardFace(const Card& c, float x, float y, float w, float h) {
    Rectangle rec = { x, y, w, h };
    DrawRectangleRounded(rec, 0.10f, 8, CARD_FACE);

    Color col = (c.getSuit() == 1 || c.getSuit() == 2) ? RED_SUIT : BLACK_SUIT;
    const char* rt = RankText(c.getRank());

    // top-left corner
    DrawText(rt, (int)(x + 10), (int)(y + 8), 26, col);
    DrawSuit(c.getSuit(), x + 20, y + 50, 9);

    // large suit in the middle
    DrawSuit(c.getSuit(), x + w / 2.0f, y + h / 2.0f + 6, 26);

    // bottom-right corner
    int tw = MeasureText(rt, 26);
    DrawText(rt, (int)(x + w - 10 - tw), (int)(y + h - 34), 26, col);
}

// Draws a card applying the flip animation (horizontal scale).
static void DrawCardAnimated(const CardView& v, float x, float y) {
    if (v.flip >= 1.0f) { DrawCardFace(v.card, x, y, CARD_W, CARD_H); return; }
    if (v.flip <= 0.0f) { DrawCardBack(x, y, CARD_W, CARD_H);        return; }

    float scale = fabsf(cosf(PI * v.flip));
    if (scale < 0.04f) scale = 0.04f;
    float w = CARD_W * scale;
    float nx = x + (CARD_W - w) / 2.0f;

    if (v.flip < 0.5f) DrawCardBack(nx, y, w, CARD_H);
    else               DrawCardFace(v.card, nx, y, w, CARD_H);
}

// ----------------------------------------------------------------------------
//  Buttons
// ----------------------------------------------------------------------------
static bool Button(Rectangle r, const char* label, Color base, bool enabled) {
    Vector2 m = GetMousePosition();
    bool hover = enabled && CheckCollisionPointRec(m, r);

    Color fill = base;
    if (!enabled)   fill = Fade(base, 0.35f);
    else if (hover) fill = ColorBrightness(base, 0.25f);

    DrawRectangleRounded(r, 0.30f, 10, fill);
    DrawRectangleRoundedLines(r, 0.30f, 10, Fade(BLACK, 0.35f));

    int fs = 24;
    int tw = MeasureText(label, fs);
    DrawText(label, (int)(r.x + (r.width - tw) / 2), (int)(r.y + (r.height - fs) / 2),
        fs, enabled ? RAYWHITE : Fade(RAYWHITE, 0.5f));

    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// ----------------------------------------------------------------------------
//  Result icons
// ----------------------------------------------------------------------------
static void DrawCheck(float cx, float cy, float r, float a) {
    DrawCircle((int)cx, (int)cy, r, Fade(WIN_GREEN, a));
    DrawCircleLines((int)cx, (int)cy, r, Fade(WHITE, a * 0.8f));
    float th = r * 0.16f;
    DrawLineEx({ cx - r * 0.42f, cy + r * 0.02f }, { cx - r * 0.10f, cy + r * 0.34f }, th, Fade(WHITE, a));
    DrawLineEx({ cx - r * 0.10f, cy + r * 0.34f }, { cx + r * 0.45f, cy - r * 0.32f }, th, Fade(WHITE, a));
}

static void DrawCross(float cx, float cy, float r, float a) {
    DrawCircle((int)cx, (int)cy, r, Fade(LOSE_RED, a));
    DrawCircleLines((int)cx, (int)cy, r, Fade(WHITE, a * 0.8f));
    float th = r * 0.16f;
    DrawLineEx({ cx - r * 0.36f, cy - r * 0.36f }, { cx + r * 0.36f, cy + r * 0.36f }, th, Fade(WHITE, a));
    DrawLineEx({ cx + r * 0.36f, cy - r * 0.36f }, { cx - r * 0.36f, cy + r * 0.36f }, th, Fade(WHITE, a));
}

static void DrawPushIcon(float cx, float cy, float r, float a) {
    DrawCircle((int)cx, (int)cy, r, Fade(GOLD_TRIM, a));
    DrawCircleLines((int)cx, (int)cy, r, Fade(WHITE, a * 0.8f));
    float th = r * 0.16f;
    DrawLineEx({ cx - r * 0.40f, cy - r * 0.16f }, { cx + r * 0.40f, cy - r * 0.16f }, th, Fade(WHITE, a));
    DrawLineEx({ cx - r * 0.40f, cy + r * 0.16f }, { cx + r * 0.40f, cy + r * 0.16f }, th, Fade(WHITE, a));
}

// ============================================================================
//  Game
// ============================================================================
class BlackjackGUI {
private:
    Deck deck;
    Hand playerHand;
    Hand dealerHand;
    std::vector<CardView> playerViews;
    std::vector<CardView> dealerViews;

    GameState   state;
    RoundResult result;

    float stateTimer;    // time spent in the current state
    float resultAnim;    // 0..1 for the final icon fade-in
    int   roundsWon;
    int   roundsLost;
    bool  quit;
    bool  openingBJ;     // 21 on the opening deal -> the dealer does not play

public:
    BlackjackGUI() : state(STATE_DEALING), result(RESULT_NONE),
        stateTimer(0.0f), resultAnim(0.0f),
        roundsWon(0), roundsLost(0), quit(false), openingBJ(false) {
        startRound();
    }

    bool wantsQuit() const { return quit; }

    // ---- new round ------------------------------------------------------
    void startRound() {
        deck.reset();
        playerHand.clear();
        dealerHand.clear();
        playerViews.clear();
        dealerViews.clear();

        // Alternating deal: player, dealer, player, dealer (face down)
        Card p1 = deck.dealCard();
        Card d1 = deck.dealCard();
        Card p2 = deck.dealCard();
        Card d2 = deck.dealCard();

        playerHand.addCard(p1); playerViews.push_back(CardView(p1, 0.00f, false));
        dealerHand.addCard(d1); dealerViews.push_back(CardView(d1, DEAL_GAP, false));
        playerHand.addCard(p2); playerViews.push_back(CardView(p2, DEAL_GAP * 2, false));
        dealerHand.addCard(d2); dealerViews.push_back(CardView(d2, DEAL_GAP * 3, true));

        state = STATE_DEALING;
        result = RESULT_NONE;
        stateTimer = 0.0f;
        resultAnim = 0.0f;
        openingBJ = false;
    }

    // ---- positions ------------------------------------------------------
    Vector2 slotPos(int index, int total, float rowY) const {
        float totalW = total * CARD_W + (total - 1) * CARD_GAP;
        float startX = (SCREEN_W - totalW) / 2.0f;
        return { startX + index * (CARD_W + CARD_GAP), rowY };
    }

    Vector2 currentPos(const CardView& v, int index, int total, float rowY) const {
        Vector2 target = slotPos(index, total, rowY);
        float e = EaseOut(v.t);
        return { DECK_POS.x + (target.x - DECK_POS.x) * e,
                 DECK_POS.y + (target.y - DECK_POS.y) * e };
    }

    bool allLanded() const {
        for (size_t i = 0; i < playerViews.size(); i++) if (!playerViews[i].landed()) return false;
        for (size_t i = 0; i < dealerViews.size(); i++) if (!dealerViews[i].landed()) return false;
        return true;
    }

    // ---- update ---------------------------------------------------------
    void advanceAnimations(float dt) {
        for (size_t i = 0; i < playerViews.size(); i++) {
            CardView& v = playerViews[i];
            if (v.delay > 0.0f) { v.delay -= dt; continue; }
            v.t += dt * DEAL_SPEED;  if (v.t > 1.0f) v.t = 1.0f;
        }
        for (size_t i = 0; i < dealerViews.size(); i++) {
            CardView& v = dealerViews[i];
            if (v.delay > 0.0f) { v.delay -= dt; continue; }
            v.t += dt * DEAL_SPEED;  if (v.t > 1.0f) v.t = 1.0f;
        }
    }

    void revealDealerCard() {
        for (size_t i = 0; i < dealerViews.size(); i++) dealerViews[i].faceDown = false;
    }

    void update(float dt) {
        stateTimer += dt;
        advanceAnimations(dt);

        // progressive flip of the revealed cards
        for (size_t i = 0; i < dealerViews.size(); i++) {
            CardView& v = dealerViews[i];
            if (!v.faceDown && v.flip < 1.0f) {
                v.flip += dt * FLIP_SPEED;
                if (v.flip > 1.0f) v.flip = 1.0f;
            }
        }

        switch (state) {

        case STATE_DEALING:
            if (allLanded() && stateTimer > 0.35f) {
                // Opening blackjack: the round ends right away
                if (playerHand.getValue() == 21 || dealerHand.getValue() == 21) {
                    openingBJ = true;
                    revealDealerCard();
                    setState(STATE_DEALER_FLIP);
                }
                else {
                    setState(STATE_PLAYER_TURN);
                }
            }
            break;

        case STATE_PLAYER_TURN:
            if (IsKeyPressed(KEY_H)) hit();
            if (IsKeyPressed(KEY_S)) stand();
            break;

        case STATE_PLAYER_DRAW:
            if (allLanded()) {
                if (playerHand.isBust()) {
                    revealDealerCard();          // dealer does not play if you busted
                    setState(STATE_DEALER_FLIP);
                }
                else {
                    setState(STATE_PLAYER_TURN);
                }
            }
            break;

        case STATE_DEALER_FLIP:
            if (dealerViews.back().flip >= 1.0f && stateTimer > 0.55f) {
                if (playerHand.isBust() || openingBJ) {
                    finishRound();
                }
                else {
                    setState(STATE_DEALER_TURN);
                }
            }
            break;

        case STATE_DEALER_TURN:
            if (allLanded() && stateTimer > 0.55f) {
                // same rule as the original Dealer class: hit while under 17
                if (dealerHand.getValue() < 17 && !dealerHand.isBust()) {
                    Card c = deck.dealCard();
                    dealerHand.addCard(c);
                    dealerViews.push_back(CardView(c, 0.0f, false));
                    stateTimer = 0.0f;
                }
                else {
                    finishRound();
                }
            }
            break;

        case STATE_ROUND_OVER:
            resultAnim += dt * 2.2f;
            if (resultAnim > 1.0f) resultAnim = 1.0f;
            if (IsKeyPressed(KEY_Y)) startRound();
            if (IsKeyPressed(KEY_N)) quit = true;
            break;
        }
    }

    void setState(GameState s) { state = s; stateTimer = 0.0f; }

    void hit() {
        Card c = deck.dealCard();
        playerHand.addCard(c);
        playerViews.push_back(CardView(c, 0.0f, false));
        setState(STATE_PLAYER_DRAW);
    }

    void stand() {
        revealDealerCard();
        setState(STATE_DEALER_FLIP);
    }

    // Same criteria as determineWinner() in the console version.
    void finishRound() {
        int pv = playerHand.getValue();
        int dv = dealerHand.getValue();

        if (playerHand.isBust())      result = RESULT_LOSE;
        else if (dealerHand.isBust()) result = RESULT_WIN;
        else if (pv > dv)             result = RESULT_WIN;
        else if (dv > pv)             result = RESULT_LOSE;
        else                          result = RESULT_PUSH;

        if (result == RESULT_WIN)  roundsWon++;
        if (result == RESULT_LOSE) roundsLost++;

        resultAnim = 0.0f;
        setState(STATE_ROUND_OVER);
    }

    // ---- drawing --------------------------------------------------------
    void drawTable() const {
        ClearBackground(FELT_DARK);
        DrawCircleGradient(SCREEN_W / 2, SCREEN_H / 2 - 40, 620, FELT_LIGHT, FELT_DARK);

        // decorative table rings
        DrawRing({ (float)SCREEN_W / 2, (float)SCREEN_H / 2 - 40 }, 470, 473, 0, 360, 64, Fade(GOLD_TRIM, 0.30f));
        DrawRing({ (float)SCREEN_W / 2, (float)SCREEN_H / 2 - 40 }, 486, 488, 0, 360, 64, Fade(GOLD_TRIM, 0.18f));

        // deck
        for (int i = 4; i >= 0; i--) {
            DrawCardBack(DECK_POS.x + i * 2.0f, DECK_POS.y - i * 2.0f, CARD_W, CARD_H);
        }
        DrawText(TextFormat("%d", deck.cardsRemaining()),
            (int)DECK_POS.x + 34, (int)DECK_POS.y + CARD_H + 8, 18, Fade(RAYWHITE, 0.75f));
    }

    void drawLabel(const char* text, int value, bool showValue, float y) const {
        DrawText(text, 40, (int)y, 24, Fade(RAYWHITE, 0.85f));
        if (showValue) {
            const char* v = TextFormat("%d", value);
            DrawRectangleRounded({ 40, y + 32, 74, 40 }, 0.35f, 8, Fade(BLACK, 0.35f));
            int tw = MeasureText(v, 30);
            DrawText(v, (int)(40 + (74 - tw) / 2), (int)(y + 37), 30, GOLD_TRIM);
        }
    }

    void drawHands() const {
        bool dealerHidden = (dealerViews.size() > 0 && dealerViews.back().flip < 1.0f);

        // Dealer on top
        for (size_t i = 0; i < dealerViews.size(); i++) {
            Vector2 p = currentPos(dealerViews[i], (int)i, (int)dealerViews.size(), DEALER_ROW_Y);
            DrawRectangleRounded({ p.x + 5, p.y + 8, CARD_W, CARD_H }, 0.10f, 8, Fade(BLACK, 0.28f));
            DrawCardAnimated(dealerViews[i], p.x, p.y);
        }

        // Player at the bottom
        for (size_t i = 0; i < playerViews.size(); i++) {
            Vector2 p = currentPos(playerViews[i], (int)i, (int)playerViews.size(), PLAYER_ROW_Y);
            DrawRectangleRounded({ p.x + 5, p.y + 8, CARD_W, CARD_H }, 0.10f, 8, Fade(BLACK, 0.28f));
            DrawCardAnimated(playerViews[i], p.x, p.y);
        }

        drawLabel("DEALER", dealerHand.getValue(), !dealerHidden, DEALER_ROW_Y);
        drawLabel("YOUR HAND", playerHand.getValue(), true, PLAYER_ROW_Y);
    }

    void drawHud() const {
        DrawText("BLACKJACK", 40, 24, 30, GOLD_TRIM);
        DrawText(TextFormat("Wins: %d   Losses: %d", roundsWon, roundsLost),
            40, 60, 18, Fade(RAYWHITE, 0.7f));
    }

    void drawControls() {
        float by = 600.0f;
        bool canAct = (state == STATE_PLAYER_TURN);

        Rectangle bHit = { SCREEN_W / 2.0f - 230, by, 200, 58 };
        Rectangle bStand = { SCREEN_W / 2.0f + 30, by, 200, 58 };

        if (Button(bHit, "HIT  (H)", { 32, 132, 200, 255 }, canAct) && canAct) hit();
        if (Button(bStand, "STAND  (S)", { 176, 96, 32, 255 }, canAct) && canAct) stand();

        if (state == STATE_DEALER_TURN || state == STATE_DEALER_FLIP) {
            const char* msg = "Dealer's turn...";
            int tw = MeasureText(msg, 22);
            DrawText(msg, (SCREEN_W - tw) / 2, (int)by + 70, 22, Fade(RAYWHITE, 0.8f));
        }
    }

    void drawResultOverlay() {
        float a = resultAnim;
        DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.62f * a));

        float cx = SCREEN_W / 2.0f;
        float cy = 250.0f;
        float r = 88.0f * (0.6f + 0.4f * a);

        const char* title = "";
        const char* sub = "";
        if (result == RESULT_WIN) {
            DrawCheck(cx, cy, r, a);
            title = "YOU WIN!";
            sub = dealerHand.isBust() ? "Dealer busted" : "Your hand is higher";
        }
        else if (result == RESULT_LOSE) {
            DrawCross(cx, cy, r, a);
            title = "YOU LOSE";
            sub = playerHand.isBust() ? "You busted" : "Dealer has a higher hand";
        }
        else {
            DrawPushIcon(cx, cy, r, a);
            title = "PUSH";
            sub = "Same total - nobody wins";
        }

        int tw = MeasureText(title, 56);
        DrawText(title, (int)(cx - tw / 2), (int)(cy + r + 24), 56, Fade(RAYWHITE, a));

        int sw = MeasureText(sub, 22);
        DrawText(sub, (int)(cx - sw / 2), (int)(cy + r + 88), 22, Fade(RAYWHITE, a * 0.8f));

        const char* score = TextFormat("You: %d          Dealer: %d",
            playerHand.getValue(), dealerHand.getValue());
        int cw = MeasureText(score, 26);
        DrawText(score, (int)(cx - cw / 2), (int)(cy + r + 126), 26, Fade(GOLD_TRIM, a));

        Rectangle bAgain = { cx - 230, 560, 220, 62 };
        Rectangle bQuit = { cx + 10,  560, 220, 62 };

        if (a > 0.7f) {
            if (Button(bAgain, "PLAY AGAIN", WIN_GREEN, true)) startRound();
            if (Button(bQuit, "QUIT", { 120, 120, 130, 255 }, true)) quit = true;

            const char* hint = "Y = play again     N / ESC = quit";
            int hw = MeasureText(hint, 18);
            DrawText(hint, (int)(cx - hw / 2), 636, 18, Fade(RAYWHITE, 0.55f));
        }
    }

    void draw() {
        drawTable();
        drawHands();
        drawHud();

        if (state == STATE_ROUND_OVER) drawResultOverlay();
        else                           drawControls();
    }
};

// ============================================================================
int main() {
    // Seed the RNG once for the whole session.
    srand((unsigned int)time(0));

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_W, SCREEN_H, "Blackjack - ENSC 251");
    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);

    BlackjackGUI game;

    while (!WindowShouldClose() && !game.wantsQuit()) {
        game.update(GetFrameTime());

        BeginDrawing();
        game.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
