#!/usr/bin/osascript

on run argv
    set dir to quoted form of (first item of argv)
    set clients to (second item of argv) as integer
    repeat with i from 1 to clients
        tell app "Terminal" to do script "cd " & dir & " && ./client 127.0.0.1 9898 tests/input" & i & ".txt"
    end repeat
end run