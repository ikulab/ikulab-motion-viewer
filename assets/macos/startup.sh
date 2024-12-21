#!/bin/bash

# このスクリプトが格納されているディレクトリ
script_dir="$(dirname "$(realpath "${0}")")"
# ikulab-motion-viewerのバイナリが格納されているディレクトリ
binary_dir="${script_dir}"
# ikulab-motion-viewer.app/Contentsのディレクトリ
bundle_dir="$(realpath "${binary_dir}/../")"
# Resourcesディレクトリ
resources_dir="$bundle_dir/Resources"

export VK_ICD_FILENAMES="${bundle_dir}/Resources/vulkan/icd.d/MoltenVK_icd.json"

# ikulab-motion-viewerを起動
"$binary_dir/imv_version_checker" "${resources_dir}/version_info" "${binary_dir}/ikulab-motion-viewer"
