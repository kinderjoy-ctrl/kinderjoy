# Experiment 6: Git Installation & Basic Operations

## 1. Installation
- Download Git from: https://git-scm.com/downloads
- Install with default settings
- Verify installation:
```bash
git --version
```

## 2. Configure Git
```bash
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
```

## 3. Create a New Repository
```bash
mkdir my-project
cd my-project
git init
```
Output: `Initialized empty Git repository in .../my-project/.git/`

## 4. Add Files (git add)
```bash
echo "Hello World" > hello.txt
echo "This is a sample project" > readme.txt

# Add a single file
git add hello.txt

# Add all files
git add .

# Check status
git status
```

## 5. Commit Changes (git commit)
```bash
git commit -m "Initial commit - added hello.txt and readme.txt"
```

## 6. View Commit History (git log)
```bash
# View full log
git log

# View compact log
git log --oneline
```

## 7. Modify a File
```bash
echo "Updated content" >> hello.txt

# See what changed
git diff

# Stage and commit the change
git add hello.txt
git commit -m "Updated hello.txt with new content"
```

## 8. View Changes
```bash
# View all commits
git log --oneline

# View a specific commit
git show HEAD

# View file at a specific commit
git show HEAD:hello.txt
```

## 9. Other Useful Commands
```bash
# Check status of working directory
git status

# View branches
git branch

# Create and switch to a new branch
git checkout -b new-feature

# Switch back to main
git checkout main
```

## Summary of Commands
| Command | Purpose |
|---------|---------|
| `git init` | Initialize a new repository |
| `git add <file>` | Stage files for commit |
| `git commit -m "msg"` | Commit staged changes |
| `git log` | View commit history |
| `git diff` | View uncommitted changes |
| `git status` | Check working directory status |
| `git show` | View a specific commit |
