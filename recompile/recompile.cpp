
#include <windows.h>

#include <cstdlib> // system関数を使用するため
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

void main() {
    system("taskkill /F /IM GM31_Test.exe");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    // 相対パス
    const char* relativePath = "../GM31_Test.vcxproj";

    // 絶対パスを格納するためのバッファ
    char fullPath[MAX_PATH];

    // 相対パスを絶対パスに変換
    DWORD result = GetFullPathNameA(relativePath, MAX_PATH, fullPath, nullptr);

    if (result == 0) {
        std::cerr << "Failed to get full path! Error code: " << GetLastError() << std::endl;
     
    }

    std::cout << "Full path: " << fullPath << std::endl;

    // msbuild コマンドを作成
   // std::string command = "cmd /C \"msbuild \"" + std::string(fullPath) + "\" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild\"";
    // result = system("\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\amd64\\MSBuild.exe\" ../GM31_Test.vcxproj /p:Configuration=Debug /p:Platform=x64 /t:Rebuild");
    result = system("\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\amd64\\MSBuild.exe\" GM31_Test.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild");

    //int rebuildResult = system(command.c_str());

    if (result != 0) {
        std::cerr << "Rebuild failed! Error code: " << result << std::endl;
    }
    else {
        std::cout << "Rebuild successful!" << std::endl;
 //const char* newexe = "..\\x64\\Debug\\GM31_Test.exe"; // 実行ファイルの名前やパスを指定

        try {//ビルド後にexeファイルをカレントディレクトリにコピー
            // コピー元とコピー先のパスを指定
            fs::path source = "x64/Release/GM31_Test.exe";
            // カレントディレクトリを取得
            fs::path currentDir = fs::current_path();

            // コピー先はカレントディレクトリにファイル名を付加したもの
            fs::path destination = currentDir / source.filename();
            // ファイルをコピー
            fs::copy_file(source, destination, fs::copy_options::overwrite_existing);

        }
        catch (const std::filesystem::filesystem_error& e) {

        }
        const char* newexe = "GM31_Test.exe"; // 実行ファイルの名前やパスを指定
    // system関数でプロセスを起動
    int result = std::system(newexe);
    }
   
}