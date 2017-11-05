CS485 -- Rupal Shah

My implementation of Program 3 does the following:

- The user may input multiple lines in the form of standard input
- Each input line should abide by one of the following requirements:
	+ start with "-c" then be followed by any displayable character
	+ start with "-d" then be followed by any decimal value
	+ solely include 2-digit hexadecimal values
- These inputs can include multiple spaces and comments, which the output will ignore
- The output converts the given lines of input into characters
- If the user inputs invalid values, an error message will output, specifying which decimal or hexadecimal was invalid
- I have included a directory entitled "tests" -> this includes test (input) files and their corresponding output files

Limitations:
- Rather than each line being able to take in 1024 characters and infinite lines, my implementation can only handle 1024 inputted characters total.
