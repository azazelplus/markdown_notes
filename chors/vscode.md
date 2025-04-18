
# vscode的配置结构:

优先级: 默认设置(--)<全局设置(`C:\Users\<YourUsername>\AppData\Roaming\Code\User\settings.json`)<工作区设置<文件夹设置.



### 1\. **全局用户设置 (User Settings)**

这个文件存储了你所有的全局配置，适用于你所有的 VSCode 项目和工作区。修改这个文件就会影响你 VSCode 的整体行为。

-   **Windows**：
    
    -   `C:\Users\<YourUsername>\AppData\Roaming\Code\User\settings.json`
        
-   **Linux**：
    
    -   `/home/<YourUsername>/.config/Code/User/settings.json`
        
-   **macOS**：
    
    -   `/Users/<YourUsername>/Library/Application Support/Code/User/settings.json`
        

### 2\. **工作区设置 (Workspace Settings)**

这个文件是为当前项目（工作区）定制的配置。它只会影响当前打开的项目，其他项目不受影响。你可以在每个项目的 `.vscode` 文件夹下找到这个文件。

-   **路径**：
    
    -   `<你的项目目录>/.vscode/settings.json`
        

### 3\. **文件夹设置 (Folder Settings)**

你可以为某个文件夹（不一定是完整的工作区）配置特定的设置。这些设置会优先级高于全局用户设置，但低于工作区设置。

-   **路径**：
    
    -   `<你的文件夹路径>/.vscode/settings.json`
        

### 4\. **默认设置 (Default Settings)**

这个文件不太可能被你直接编辑，但是 VSCode 内部有一份“默认设置”，所有的设置都会从这个文件继承。你只能通过在用户设置文件（`settings.json`）中覆盖这些默认值来修改它。

-   **路径**：
    
    -   内部存储，不需要关心
        

### 如何快速定位这些文件？

1.  **通过 VSCode 直接打开**：
    
    -   按 `Ctrl + Shift + P`，然后输入 `Preferences: Open Settings (JSON)` 打开全局 `settings.json`，或者选择 `Preferences: Open Workspace Settings (JSON)` 打开当前工作区的设置。
        
2.  自己去对应路径找: 全局设置(`C:\Users\<YourUsername>\AppData\Roaming\Code\User\settings.json`), 工作区和文件夹的就在文件夹主目录.vscode.

