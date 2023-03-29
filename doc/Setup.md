# build

## Mac

基本的に`$ command`はシェルで実行するコマンドを示します。
例えば、`$ echo hello!!`の場合はターミナルで`echo hello!!`と入力しEnterをタイプしてください。
「`$`」は入力しないで下さい。

作業ディレクトリはikulab-motion-viewerのリポジトリルートとします。
`git clone`でクローンしたディレクトリに`cd`すればOKです。
```
$ cd Documents # <-- ikulab-motion-viewerフォルダを置きたい場所
$ git clone https://github.com/ikulab/ikulab-motion-viewer.git
$ cd ikulab-motion-viewer
```

### ビルドツールのインストール

- homebrewで以下のパッケージをインストール
  - `brew install clang cmake ninja`

### 依存ライブラリのインストール

- GLFWのインストール
  - `brew install glfw`
- Vulkan SDKのインストール<br>
  -  [ダウンロードサイト](https://vulkan.lunarg.com/sdk/home#mac) からmac版の`1.2.198.1`をインストールする
- shaderc_combinedのインストール (方法1, 方法2の**どちらか**を実行してください)
  - 方法1: shadercのビルド (時間は掛かるが確実な方法)
    - シェルから`install_external_library.sh`を実行
    - `$ ./utils/scripts/install_external_library.sh`
    - ↑「$」はプロンプト文字列なので入力しなくてOK
  - 方法2: プリビルドバイナリのダウンロード
    - [Dropboxの共有フォルダ](https://www.dropbox.com/scl/fo/n7vfm3n3js8bdi7bh0er8/h?dl=0&rlkey=vshzi0r4gntzyn8f4i1ixt3a7) から.aファイルをダウンロード
      - M1, M2 macなら`shaderc_combined_arm.a`
      - Intel macなら`shaderc_combined_x86_64.a`
    - ダウンロードした.aファイルをikulab-motion-viewerの`libs/shaderc/shaderc_combined.a`にコピーする
      - `shaderc`フォルダは無いので作ること
      - 必ずファイル名を`shaderc_combined.a`に変更すること

### ビルド

- CMakeファイルの生成
  - `$ ./utils/scripts/config_cmake.sh`
  - `build`というディレクトリが作成される
- ビルド
  - `$ ./utils/scripts/build.sh`

ビルドをやり直す時は、`build.sh`の実行のみでOK。
ただし、一回`build`ディレクトリを消して`config_cmake.sh`からやり直さないとうまく行かないケースもある(CMakeキャッシュ変数の変更など)。

### アセット配置

Note: インストーラーが完成したら自動化される作業です

- ikulab-motion-viewerのリソースディレクトリを作成する
  - `$ sudo mkdir -p /usr/local/share/ikulab-motion-viewer`
- リソースディレクトリにフォントをコピーする
  - `fonts`ディレクトリを作成
    - `$ sudo mkdir -p /usr/local/share/ikulab-motion-viewer/fonts`
  - フォントのコピー
    - `$ sudo cp assets/fonts/NotoSansJP-Medium.otf /usr/local/share/ikulab-motion-viewer/fonts`

### 実行

`$ ./build/app/ikulab-motion-viewer`

---

## Windows

Intel CPUのWindowsを使っている場合、[DropBoxの共有フォルダ](https://www.dropbox.com/home/Research/Public)から`ikulab-motion-viewer.exe`をダウンロードし実行することができます。
この場合でも以下の「アセット配置」を実行する必要があります。

### ビルドツールのインストール

Visual Studio Community 2022をインストール<br>
https://visualstudio.microsoft.com/ja/

インストール項目は「C/C++ デスクトップアプリ開発」みたいなやつ

### 依存ライブラリのインストール

- shaderc_combined.lib
  - [ここ](https://storage.googleapis.com/shaderc/badges/build_link_windows_vs2017_release.html) からプリビルドバイナリをダウンロード
  - install.zipを解凍し、`install\lib\shaderc_combined.lib`をikulab-motion-viewerの`libs/shaderc/shaderc_combined.lib`にコピー
    - `shaderc`ディレクトリは無いので作成すること

### ビルド

- CMakeファイルの生成
  - `$ .\utils\scripts\config_cmake_win.bat`
  - `build`というディレクトリが作成される
- ビルド
  - `$ .\utils\scripts\build_win.bat`

ビルドをやり直す時は、`build_win.bat`の実行のみでOK。
ただし、一回`build`ディレクトリを消して`config_cmake_win.bat`からやり直さないとうまく行かないケースもある(CMakeキャッシュ変数の変更など)。

### アセット配置

Note: インストーラーが完成したら自動化される作業です

- ディレクトリ`C:\Users\${UserName}\.ikulab-motion-viewer`を作成
  - `${UserName}`は自分のユーザー名
- ディレクトリ`C:\Users\${UserName}\.ikulab-motion-viewer\fonts`を作成
- フォントのコピー
  - `assets\fonts\NotoSansJP-Medium.otf`を`C:\Users\${UserName}\.ikulab-motion-viewer\fonts`にコピー
  - 共有フォルダからダウンロードする場合も`NotoSansJP-Medium.otf`を`C:\Users\${UserName}\.ikulab-motion-viewer\fonts`にコピーする

- ikulab-motion-viewerのリソースディレクトリを作成する
  - `$ sudo mkdir -p /usr/local/share/ikulab-motion-viewer`
- リソースディレクトリにフォントをコピーする
  - `fonts`ディレクトリを作成
    - `$ sudo mkdir -p /usr/local/share/ikulab-motion-viewer/fonts`
  - フォントのコピー
    - `$ sudo cp assets/fonts/NotoSansJP-Medium.otf /usr/local/share/ikulab-motion-viewer/fonts`

### 実行

`build/app/Debug/ikulab-motion-viewer.exe`を実行
