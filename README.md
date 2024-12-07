
# Лабораторна робота №3

Ми продовжили симуляцію алгоритмів, додавши нові алгоритми, та функціальності.




#### Були отримані наступні результати:
#### FCFS Scheduling
| Process | Arrival TimeBurst | Time | Wait Time | Completion TimeTurnaround | Time |
|---------|-------------------|------|-----------|---------------------------|------|
| 1       | 2                 | 18   | 0         | 20                        | 18   |
| 7       | 2                 | 13   | 18        | 33                        | 31   |
| 4       | 4                 | 7    | 29        | 40                        | 36   |
| 2       | 5                 | 6    | 35        | 46                        | 41   |
| 6       | 7                 | 12   | 39        | 58                        | 51   |
| 3       | 8                 | 18   | 50        | 76                        | 68   |
| 5       | 8                 | 17   | 68        | 93                        | 85   |

#### Round Robin Scheduling
| Process | Arrival TimeBurst | Time | Wait Time | Completion TimeTurnaround | Time |
|---------|-------------------|------|-----------|---------------------------|------|
| 1       | 2                 | 18   | 73        | 93                        | 91   |
| 7       | 2                 | 13   | 78        | 93                        | 91   |
| 4       | 4                 | 7    | 82        | 93                        | 89   |
| 2       | 5                 | 6    | 82        | 93                        | 88   |
| 6       | 7                 | 12   | 74        | 93                        | 86   |
| 3       | 8                 | 18   | 67        | 93                        | 85   |
| 5       | 8                 | 17   | 68        | 93                        | 85   |


#### Shortest Job First (SJF)
| Process | Arrival TimeBurst | Time | Wait Time | Completion TimeTurnaround | Time |
|---------|-------------------|------|-----------|---------------------------|------|
| 1       | 2                 | 18   | 0         | 0                         | -2   |
| 7       | 2                 | 13   | 0         | 0                         | -2   |
| 4       | 4                 | 7    | 0         | 0                         | -4   |
| 2       | 5                 | 6    | 0         | 0                         | -5   |
| 6       | 7                 | 12   | 0         | 0                         | -7   |
| 3       | 8                 | 18   | 0         | 0                         | -8   |
| 5       | 8                 | 17   | 0         | 0                         | -8   |

#### Priority Scheduling with Aging

| Process | Arrival TimeBurst | Time | Wait Time | Completion TimeTurnaround | Time |
|---------|-------------------|------|-----------|---------------------------|------|
| 1       | 2                 | 18   | 0         | 0                         | -2   |
| 7       | 2                 | 13   | 0         | 0                         | -2   |
| 4       | 4                 | 7    | 0         | 0                         | -4   |
| 2       | 5                 | 6    | 0         | 0                         | -5   |
| 6       | 7                 | 12   | 0         | 0                         | -7   |
| 3       | 8                 | 18   | 0         | 0                         | -8   |
| 5       | 8                 | 17   | 0         | 0                         | -8   |
