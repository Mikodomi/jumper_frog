# jumper_frog
Exit with F1.

# Movement
Move with w,a,s,d or k,h,j,l.

# configuration
Configuration is done through config.txt (has to be named __exactly__ that). 
Every command like `FROG`, `ROADS` must be followed by a newline character.
- ```txt
    ROADS
    n
    roads[0].posy roads[0].lanes
    roads[1].posy roads[1].lanes
    .
    .
    .
    roads[n-1].posy roads[n-1].lanes```

In place of roads[0..n-1].posy place an integer denoting the y-coordinate of the road (counting from the top e.g 2 is 2nd row from the top).
Similarly with lanes, just put the amount of lanes of the current road.
