#!/bin/bash

# このスクリプトが格納されているディレクトリ
script_dir="$(dirname "$(realpath "$0")")"
# ikulab-motion-viewerのバイナリが格納されているディレクトリ
binary_dir="$script_dir"

# alias対策で、確実にBSD sedを使用するためのパス
sed_command="/usr/bin/sed"

# 表示するポップアップのタイトル
popup_title="ikulab-motion-viewer スタートアップ"

# --------------------------------------------------
# 関数定義
# --------------------------------------------------

function is_vulkan_installed() {
	vulkan_library_path=$(find /usr/lib /usr/local/lib -name "libvulkan.dylib")
	if [ -n "$vulkan_library_path" ]; then
		return 0
	fi
	return 1
}

function print_imv_vulkan_version() {
	otool -L "$binary_dir/ikulab-motion-viewer" | grep 'libvulkan.*\.dylib' | "$sed_command" -E 's/^.*libvulkan\.([0-9\.]+)\..*$/\1/'
}

function print_installed_vulkan_version() {
	vulkan_library_path=$(find /usr/lib /usr/local/lib -name "libvulkan.dylib")
	link_target1="$(dirname "$vulkan_library_path")/$(readlink "$vulkan_library_path")"
	link_target2="$(dirname "$vulkan_library_path")/$(readlink "$link_target1")"

	echo "$link_target2" | "$sed_command" -E 's/.*libvulkan\.([0-9\.]+)\.dylib/\1/'
}

# 実行環境にインストールされているVulkanのバージョンと引数に渡されたバージョンが一致しているか判定する。
function is_required_version_installed() {
	required_vulkan_version=$1
	installed_vulkan_version=$(print_installed_vulkan_version)

	if [ "$required_vulkan_version" = "$installed_vulkan_version" ]; then
		return 0
	fi
	return 1
}

function show_download_suggest_popup() {
	vulkan_sdk_download_url="https://sdk.lunarg.com/sdk/download/1.3.280.0/mac/vulkansdk-macos-1.3.280.0.dmg"

	popup_content=""
	popup_content="${popup_content}libvulkan.dylib がインストールされていません。\n"
	popup_content="${popup_content}以下のリンクからVulkan SDKインストーラーをダウンロードし、システムへのインストールを行って下さい。\n"
	popup_content="${popup_content}\n"
	popup_content="${popup_content}${vulkan_sdk_download_url}\n"
	popup_content="${popup_content}\n"
	popup_content="${popup_content}※インストールを行う際は、必ず 'System Global Installation' にチェックを入れて下さい。"

	btn_txt_open_link="リンクを開く"
	btn_txt_close="閉じる"
	popup_result=$(osascript -e "display dialog \"$popup_content\" buttons {\"$btn_txt_open_link\", \"$btn_txt_close\"} default button \"$btn_txt_close\" with icon stop with title \"$popup_title\"")

	if [[ "$popup_result" = *"$btn_txt_open_link"* ]]; then
		open "$vulkan_sdk_download_url"
	fi
}

# libvulkan.dylibのバージョンが異なる場合の警告ポップアップを表示する。
# 今後表示しない場合は、バイナリファイルと同じディレクトリに.dont_show_vulkan_version_warningファイルを作成する。
# .dont_show_vulkan_version_warningファイルが存在する場合は、ポップアップを表示しない。
# 戻り値:
#  0: 続行する場合 (今後表示しない場合も含む)
#  1: キャンセルする場合
function show_compatibility_warning_popup() {
	if [ -f "$binary_dir/.dont_show_vulkan_version_warning" ]; then
		return 0
	fi

	popup_content=""
	popup_content="${popup_content}libvulkan.dylib のバージョンが異なります。\n"
	popup_content="${popup_content}\n"
	popup_content="${popup_content}ikulab-motion-viewerが使用するバージョン: $(print_imv_vulkan_version)\n"
	popup_content="${popup_content}インストールされているバージョン: $(print_installed_vulkan_version)\n"
	popup_content="${popup_content}\n"
	popup_content="${popup_content}※こちらのリンクからバージョンごとのVulkan SDKをダウンロードできます:\n"
	popup_content="${popup_content}https://vulkan.lunarg.com/sdk/home\n"
	popup_content="${popup_content}\n"
	popup_content="${popup_content}続行すると、正常に動作しない可能性があります。\n"

	btn_txt_cancel="終了する"
	btn_txt_continue="続行"
	btn_txt_dont_show_again="続行し今後表示しない"

	popup_result=$(osascript -e "display dialog \"$popup_content\" buttons {\"$btn_txt_cancel\", \"$btn_txt_continue\", \"$btn_txt_dont_show_again\"} default button \"$btn_txt_cancel\" with icon caution with title \"$popup_title\"")

	echo "$popup_result"

	if [[ "$popup_result" = *"$btn_txt_continue"* ]]; then
		return 0
	elif [[ "$popup_result" = *"$btn_txt_cancel"* ]]; then
		return 1
	elif [[ "$popup_result" = *"$btn_txt_dont_show_again"* ]]; then
		touch "$binary_dir/.dont_show_vulkan_version_warning"
		return 0
	fi
}


# --------------------------------------------------
# メイン処理
# --------------------------------------------------

# Vulkanがインストールされていない場合、ダウンロードを催促して終了する
if ! is_vulkan_installed; then
	show_download_suggest_popup
	exit 1
fi

required_version="$(print_imv_vulkan_version)"

# インストールされているVulkanのバージョンが異なる場合、警告する
# ユーザーの選択によっては終了する
if ! is_required_version_installed "$required_version"; then
	if ! show_compatibility_warning_popup; then
		# 警告ダイアログでキャンセルした場合
		exit 1
	fi
fi

# ikulab-motion-viewerを起動
"$binary_dir/ikulab-motion-viewer"
