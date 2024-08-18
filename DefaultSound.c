#include <windows.h>
#include <stdio.h>
#include <string.h>

// Function to set the registry value
void SetRegistryValue(LPCSTR keyPath, LPCSTR valueName, LPCSTR data) {
    HKEY hKey;
    LONG result;

    result = RegOpenKeyExA(HKEY_CURRENT_USER, keyPath, 0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS) {
        RegSetValueExA(hKey, valueName, 0, REG_SZ, (const BYTE*)data, strlen(data) + 1);
        RegCloseKey(hKey);
        printf("Successfully updated registry key: %s\n", keyPath);
    } else {
        printf("Failed to open registry key: %s\n", keyPath);
    }
}

int main() {
    char connectSoundFilePath[MAX_PATH];
    char disconnectSoundFilePath[MAX_PATH];

    // Get the current directory and append the sound file names
    GetCurrentDirectoryA(MAX_PATH, connectSoundFilePath);
    strcat(connectSoundFilePath, "\\Windows_Hardware_Insert.wav");

    GetCurrentDirectoryA(MAX_PATH, disconnectSoundFilePath);
    strcat(disconnectSoundFilePath, "\\Windows_Hardware_Remove.wav");

    // Set the connect and disconnect sounds
    SetRegistryValue("AppEvents\\Schemes\\Apps\\.Default\\DeviceConnect\\.Current", "", connectSoundFilePath);
    SetRegistryValue("AppEvents\\Schemes\\Apps\\.Default\\DeviceDisconnect\\.Current", "", disconnectSoundFilePath);

    // Notify the system that a change has been made
    SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"windows", SMTO_ABORTIFHUNG, 5000, NULL);

    // Print completion messages
    printf("USB connect sound has been updated to: %s\n", connectSoundFilePath);
    printf("USB disconnect sound has been updated to: %s\n", disconnectSoundFilePath);

    // Wait for user input before closing
    printf("Press any key to exit...\n");
    getchar();  // Keeps the console window open until a key is pressed

    return 0;
}
