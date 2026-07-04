from pathlib import Path

count = len(list(Path(".").rglob("*.ino")))

print(f"Total .ino files: {count}")