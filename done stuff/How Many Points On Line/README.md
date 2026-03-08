

```markdown
# Max Points on a Line — Algorithm Tool

A command-line tool that finds the line passing through the maximum number of points from a given set of 2D coordinates.

Built as a learning project in C++ — my first C++ program, developed entirely on a rooted Android tablet running Debian via chroot.

## What It Does

Given a set of 2D points, the algorithm:

1. **Parses** raw user input into typed coordinate pairs
2. **Generates** every possible line equation from all point pairs
3. **Eliminates** duplicate lines using epsilon-based floating point comparison
4. **Validates** every point against every line to count matches
5. **Returns** the winning line equation and how many points lie on it

```
Input:  [0.0,0.0],[1.0,1.0],[2.0,2.0],[3.0,3.0],[1.0,0.0],[2.0,3.0]

Output: Your Winner Line Equation Is y = 1x + (0)
        And The Number Of Points That Fit On It = 4
```

## The Problem

This is based on [LeetCode #149 — Max Points on a Line](https://leetcode.com/problems/max-points-on-a-line/) which is classified as a **Hard** problem.

But instead of just returning a count like LeetCode expects, this program:

- Takes real user input (not hardcoded test cases)
- Validates input format
- Returns the actual line equation (`y = mx + c` or `x = c`)
- Handles all edge cases (vertical lines, horizontal lines, negative slopes, decimal coordinates)

## How It Works

### Architecture

```
input() → FromStoP() → eqofline_generator() → validator() → output
  ↓           ↓              ↓                      ↓           ↓
raw string → points → all possible lines → count points → winner
```

### Step 1 — Input & Parsing

The parser walks through the input string character by character, extracting numbers by detecting digits, decimal points, and negative signs. It ignores brackets and splits on commas.

```
"[3.0,6.0],[7.0,5.0],[-1.6,7.4],"
  ↓
[{3.0, 6.0}, {7.0, 5.0}, {-1.6, 7.4}]
```

### Step 2 — Line Generation

For every unique pair of points, the algorithm calculates the line equation `y = mx + c` where:

```
m = (y2 - y1) / (x2 - x1)    (slope)
c = y1 - m * x1               (y-intercept)
```

Three cases are handled:

| Case | Condition | Representation |
|------|-----------|----------------|
| Normal line | `dx ≠ 0` and `dy ≠ 0` | `y = mx + c` |
| Horizontal line | `dy = 0` | `y = c` (slope = 0) |
| Vertical line | `dx = 0` | `x = c` (slope = NaN) |

### Step 3 — Duplicate Elimination

Before adding a new line, the duplicate checker compares it against all existing lines using epsilon comparison (`1e-10`) on both slope and intercept. This handles floating point precision issues where mathematically identical lines might have slightly different calculated values.

### Step 4 — Validation

Every point is tested against every unique line using the line equation. A point is considered "on the line" if:


|y - (mx + c)| < epsilon       (for normal/horizontal lines)
|x - c| < epsilon              (for vertical lines)
```

### Step 5 — Result

The line with the highest point count wins.

## Edge Cases Handled

| Edge Case | How It's Handled |
|-----------|-----------------|
| Vertical lines (undefined slope) | NaN used as slope marker, x-intercept stored |
| Horizontal lines (zero slope) | Explicit `dy == 0` check |
| Negative coordinates | Parser detects `-` sign |
| Decimal coordinates | Parser handles `.` in numbers |
| Floating point comparison | Epsilon-based comparison (`1e-10`) instead of `==` |
| Duplicate lines | Checked before adding to line collection |
| Invalid input format | Input validator rejects and re-prompts |

### Usage

### Compile

```bash
g++ how_many_p_on_l.cpp -o how_many_p_on_l

```

### Run

```bash
./how_many_p_on_l
```

### Input Format

```
[x1,y1],[x2,y2],[x3,y3],
```

- Points are enclosed in square brackets
- Coordinates separated by commas
- Points separated by commas
- **Must have a trailing comma after the last point**
- **No spaces**

### Example Session

```
IMPORTANT ----> enter points in this form : [1.0,2.0],[3.0,4.0], ...etc
don't use spaces or anything and make sure to type a coma after the last point

Enter Your Data: [0.0,0.0],[1.0,1.0],[2.0,2.0],[3.0,3.0],[4.0,4.0],[1.0,0.0],[2.0,3.0],

 ----------------------------- YOUR RESULT ----------------------------- 

Your Winner Line Equations Is y = 1x + (0)
And The Number Of Points That Fit On It = 5
```

## Benchmark Results

| Test | Points | Expected Result | Output | Status |
|------|--------|----------------|--------|--------|
| Small mixed | 12 | y = 1.5x + (0), 8 pts | y = 1.5x + (0), 8 pts | ✅ |
| Vertical + horizontal | 11 | x = 3, 6 pts | x = 3, 6 pts | ✅ |
| Negative coords | 11 | y = -2x + (0), 8 pts | y = -2x + (0), 8 pts | ✅ |
| All collinear | 7 | y = 2.5x + (0), 7 pts | y = 2.5x + (0), 7 pts | ✅ |
| Large mixed | 20 | y = 1x + (0), 10 pts | y = 1x + (0), 10 pts | ✅ |
| Stress test | 500 | y = 3x + (2), 50 pts | y = 3x + (2), 50 pts | ✅ |

## Performance

The algorithm uses a brute force approach with O(n³) time complexity:

| Points | Pairs Checked | Approximate Time* |
|--------|--------------|-------------------|
| 10 | 45 | Instant |
| 20 | 190 | Instant |
| 100 | 4,950 | < 1 second |
| 500 | 124,750 | ~5 minutes |

*Times measured on Snapdragon 662 (Galaxy Tab A7) running Debian chroot

## Technical Details

- **Language:** C++
- **Standard:** C++17
- **Dependencies:** Standard library only
- **Floating point handling:** Epsilon comparison (1e-10) for all double comparisons
- **Vertical line handling:** NaN-based slope marker with `std::isnan()` checks

## License

Do whatever you want with it. I'm learning.