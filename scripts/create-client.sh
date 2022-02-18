#!/usr/bin/osascript

on run argv
  set dir to quoted form of (first item of argv)
  tell app "Terminal" to do script "cd " & dir & " && ./client 127.0.0.1 9898"
end run