* Y-Vector Concurrency Example
* Demonstrates parallel lanes in E-Script

lane y1 process write "Lane 1: Reading telemetry" #
lane y2 process write "Lane 2: Analyzing patterns" #
lane y3 process write "Lane 3: Writing summary" #

sync lanes #

process write "All lanes synchronized!" #
