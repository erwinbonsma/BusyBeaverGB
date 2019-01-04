# Challenges

## Count to 8
* Type: HiOut, Target = 8
* Fixed: None
* Available: DATA=8
* Tutorial: Demonstrates DATA increment

## Bumpy count to 8
* Type: HiOut, Target = 8
* Fixed: Some TURNs
* Available: DATA=8
* Tutorial: Demonstrates TURNS and directional DATA increment

## Exit 1
* Type: ExitHere, Exit = [2, 9]
* Fixed: None
* Available: TURN=4, DATA=1
* Tests: Turns

## Exit 2
* Type: ExitHere, Exit = [1, 9]
* Fixed: None
* Available: TURN=8, DATA=1

## Exit 3
* Type: ExitHere, Exit = [1, 9]
* Fixed: None
* Available: TURN=2, DATA=2
* Tests: Basic shift and turn

## Exit 4
* Type: ExitHere, Exit = [2, 9]
* Fixed: TURN=9
* Available: DATA=6
* Tests: Shifts and turns

## Count to 16
* Type: HiOut, Target = 16
* Fixed: None
* Available: DATA=16, TURN=12
* Tests: Layout/route skills

## Eight ones
* Type: Sequence, Target = 1 1 1 1 1 1 1 1
* Fixed: None
* Available: TURN=15, DATA=15
* Tests: Basic shift and turn

## Countdown to -8
* Type: LoOut, Target = -8
* Fixed: None,
* Available: TURN=10, DATA=10

## Seven Ate Nine
* Type: Sequence, Target = 8 0 0 0 0 0 7
* Fixed: TURNs
* Available: DATA=26
* Tutorial: Left-shifts

## Run 100
* Type: BusyBeaver, Target >= 100
* Fixed: TURNs
* Available: DATA=6
* Tutorial: For loop

## Count to 32
* Type: HiOut, Target = 32
* Fixed: None
* Available: DATA=*, TURN=*
* Tests: For loop

## Exit X (TBD)
* Type: ExitHere
* Fixed: Some DATA, Some TURN
* Available: Some TURNs
* Test: Puzzle skills

## Four threes
* Type: Sequence: Target

## Run 1000
* Type: BusyBeaver, Target >= 1000
* Fixed: None
* Available: DATA=*, TURN=*

## Count past 1000
* Type: HiOut, Target >= 1000
* Fixed: None
* Available: DATA=*, TURN=*

## Run 10k
* Type: BusyBeaver, Target >= 10000
* Fixed: None
* Available: DATA=*, TURN=*

## Run 100k (feasible?)
* Type: BusyBeaver, Target >= 100000
* Fixed: None
* Available: DATA=*, TURN=*