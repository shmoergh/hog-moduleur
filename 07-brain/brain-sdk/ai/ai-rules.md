# AI coding agent rules

- Focus only on what's been asked, only create subtasks that are strictly related and necessary to finish the given task
- If a todo list is provided, focus only on the todo list. Stop when you're done and ask for review and confirmation. Check off points on the checklist in the todo list as you work.
- Go in small chunks so I can review and make sure everything looks good.
- Remove all temporary debug/build tasks from .vscode/tasks.json after use. Only keep permanent, project-wide tasks.
 - Always update todo lists by checking off finished tasks as work progresses.
 - In agent mode, apply changes to a single file directly without asking for permission. Ask for permission only when editing multiple files at once.
 - Always check both the header and implementation files of all C++ classes when making changes or reviewing code.