# Blackjack — Graphical Version (C++ / raylib)

Graphical front-end for the same blackjack engine used by the console version.
It reuses `Card`, `Deck` and `Hand` straight from the repo root — those files are
**not** duplicated here.

## How to run it

1. Right-click the **`gui`** folder → **Show more options** → **Open with Visual Studio**
   (open `gui`, not the repo root — the `CMakeLists.txt` lives inside `gui/`)
2. Visual Studio picks up `CMakeLists.txt` and configures itself. The **first time** it
   downloads and builds raylib (needs internet, takes ~1–2 minutes).
3. Once the toolbar shows `BlackjackGUI.exe`, press **Ctrl+F5**.

Requires Visual Studio 2022 with the **Desktop development with C++** workload
(that installs the compiler and CMake).

## Controls

| Action | Keyboard | Mouse |
|---|---|---|
| Hit | `H` | **HIT** button |
| Stand | `S` | **STAND** button |
| Play again | `Y` | **PLAY AGAIN** button |
| Quit | `N` or `ESC` | **QUIT** button |

## What it does

- Animated deal: cards fly out of the deck to their slot, one at a time.
- **Dealer's cards on top**, **yours at the bottom**, with each hand's value.
- The dealer's second card is dealt face down and flips over with an animation
  once you stand.
- End of round: **green ✓** if you win, **red ✗** if you lose, **gold =** on a push,
  plus the running score and the play-again screen.

## Files

```
repo/
├── Card.cpp/.hpp      shared with the console version, unchanged
├── Deck.cpp/.hpp      shared with the console version, unchanged
├── Hand.cpp/.hpp      shared with the console version, unchanged
└── gui/
    ├── CMakeLists.txt fetches raylib and builds everything
    ├── main.cpp       UI, animations and the game state machine
    └── README.md
```

## Design notes

- `Player`, `Dealer` and `BlackjackGame` are not used here. They do their I/O through
  `cin`/`cout`, which blocks — a game loop has to redraw ~60 times a second and can never
  wait on input. Their logic is rewritten as a state machine inside the `BlackjackGUI`
  class in `main.cpp`. The rules are unchanged: dealer hits under 17, Aces count 11 and
  drop to 1 when they would bust, highest hand at or under 21 wins.
- `main.cpp` seeds the RNG once at startup (`srand(time(0))` in `main()`), instead of
  reseeding it inside `shuffle()`. Reseeding on every shuffle would make two shuffles in
  the same second deal out an identical deck.
