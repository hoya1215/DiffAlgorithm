# DiffAlgorithm


## Algorithm History


### Version 1.
- LCS + UpperBound + DP


-> Low Accuracy

### Version 2.
- Extract unique common line
- Find common line within each unique common line range with LIS + UpperBound + DP
- Compare all line based on common line


-> High Accuracy, Almost 90% Accuracy with other DiffTools

### Version 3.

- Extract unique common line method
before : if line count is same, push common vector
after : apply LCS to common vector, extract the longest common sequence

-> Fix errors that occur in the correct order
