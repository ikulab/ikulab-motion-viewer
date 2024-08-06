# ビルド手順

## MacOS

`v1.2.0` 以降はMacOS用のスタンドアローンなアプリケーション `ikulab-motion-viewer.app` を提供しているため、[リリースページ](https://github.com/ikulab/ikulab-motion-viewer/releases) からダウンロードすることを推奨します。

---

リポジトリをcloneし、リポジトリルートにcdしてください。
次に、以下のコマンドでsubmoduleをpullしてください

```shell
$ git submodule update --init --recursive
```

以下のスクリプトでビルド及びアプリケーションのパッケージングができます

```shell
$ ./utils/scripts/build_app_package.sh
```

`build_app_package.sh` の実行が完了すると、`build/app/` 以下にアプリケーションが格納されたdmgファイルが生成されます。

---

以前のビルド手順はtag: `v1.1.0` 時点のSetup.mdを参照。<br>
https://github.com/ikulab/ikulab-motion-viewer/blob/v1.1.0/doc/Setup.md

## Windows: TODO

TODO: Windowsでvcpkgを用いたビルドの手順を書く

以前のビルド手順はtag: `v1.1.0` 時点のSetup.mdを参照。<br>
https://github.com/ikulab/ikulab-motion-viewer/blob/v1.1.0/doc/Setup.md

## Linux: TODO

TODO: Linuxでvcpkgを用いたビルドの手順を書く<br>
MEMO: 多分そんなに難しくない

以前のビルド手順はtag: `v1.1.0` 時点のSetup.mdを参照。<br>
https://github.com/ikulab/ikulab-motion-viewer/blob/v1.1.0/doc/Setup.md
