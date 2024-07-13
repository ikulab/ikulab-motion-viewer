# ビルド手順

## MacOS

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

homebrewで以下のパッケージをインストール

```
$ brew install clang cmake ninja
```

### ビルド

```
$ ./utils/scripts/build_app_package.sh
```

### 実行

`build/ikulab-motion-viewer-xxx.dmg` といった名前のdmgファイルが生成されているはずなので、Finderからダブルクリップして開く。<br>
中に入っている `ikulab-motion-viewer.app` をアプリケーションフォルダに移動させる。<br>
その後、他のMacOSアプリと同じように `ikulab-motion-viewer.app` を実行する。<br>

## Windows: TODO

TODO: Windowsでvcpkgを用いたビルドの手順を書く

以前のビルド手順はtag: `v1.1.0` 時点のSetup.mdを参照。<br>
https://github.com/ikulab/ikulab-motion-viewer/blob/v1.1.0/doc/Setup.md

## Linux: TODO

TODO: Linuxでvcpkgを用いたビルドの手順を書く<br>
MEMO: 多分そんなに難しくない

以前のビルド手順はtag: `v1.1.0` 時点のSetup.mdを参照。<br>
https://github.com/ikulab/ikulab-motion-viewer/blob/v1.1.0/doc/Setup.md
