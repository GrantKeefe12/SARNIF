# Copyright 2020 Autoware Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

list(INSERT CMAKE_MODULE_PATH 0 "${livox_sdk_vendor_DIR}/Modules")

# Point downstream packages at the vendored SDK prefix.
# Prefer the populated build-tree SDK during workspace builds, and fall back to
# the install prefix if it already contains the SDK files.
get_filename_component(_livox_sdk_vendor_install_prefix "${livox_sdk_vendor_DIR}/../../.." ABSOLUTE)
if(EXISTS "${_livox_sdk_vendor_install_prefix}/include/livox_sdk.h")
	set(livox_sdk_ROOT_DIR "${_livox_sdk_vendor_install_prefix}")
else()
	get_filename_component(_livox_sdk_workspace_root "${livox_sdk_vendor_DIR}/../../../.." ABSOLUTE)
	set(livox_sdk_ROOT_DIR "${_livox_sdk_workspace_root}/build/livox_sdk_vendor/livox-sdk_install")
endif()
