# LOGGER

## Idea behind this logger
- When using this interface library, it can work with and without a `logger_conf.json` file.
    - When using without the `logger_conf.json` file, the `root` logger will have a logger level of `trace` and all other loggers will be configured to have a logger level of `info`. There are no functions to change that behavior in the code
    - When using with a `logger_conf.json` file, one should:
        - define a `root` json object meant to depict a root logger
            - that `root` logger should define a `logger_level` and `appenders`
        - define other loggers (cannot be empty)
            - Each logger just defines a `logger_level`
        - define appenders that define where log statements print to
            - currently, only `output_file` is a supported config value
        - define a top level `logger_template` which gives full control over where tokens exist in your log output
- The `logger_conf.json` file is meant to be in a directory named `config` relative to the directory the executable using the logger is in.
    - example: if an executable exists in `some/path/to/bin/exe` then there must exist `some/path/to/config/logger_conf.json`, for the config values to be used. Else the executable uses default values outlined above.

## Tokens
- `#H`: Current calendar month depicted as a decimal, in the range of 1-12
- `#A`: Day of the month, in the range of 1-31
- `#Y`: The full year, ex. 2023
- `#t`: The current time in hours:minutes:seconds, usually denoted as HH:MM:SS
- `#k`: Milli seconds
- `#u`: Micro seconds
- `#n`: Nano seconds
- `#N`: The name the logger
- `#L`: The logger level
- `#M`: The message to be printed
- `{}`: Is the place holder used for resolving variables inline within the msg output
    - example: `info("Log {}", "statement")` resolves to `"Log statement".`
- Full Example: `[ #H-#A-#Y #t.#k.#u.#n ] [ #N ] [ #L ] #M`

### Todos:
- Right now, appenders are sort of simulated in the code. It would be beneficial at some point to actually make appender interfaces and use them to output the log statements to files
    - This way you can change what gets printed to files vs stdout, as of now, it does not do that. Each appender gets the same statement
    - Also, each appender can get different pattern layouts and thresholds
    - Also, each appender can get a file lock which would increase performace when there are a lot of appenders/loggers in the mix
- Also work to make it more of an actual .so file instead of an interface library. Or at least make a cmake option to pick between the two. I originally made it an interface library to allow printing of the file and line number a statement was in but I'm not sure that's all that necessary
- Some general code cleanup of the initialization aspect could be helpful, I'm not sure if it could be better but maybe could be.

