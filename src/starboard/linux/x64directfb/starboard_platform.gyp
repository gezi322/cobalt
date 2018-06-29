# Copyright 2015 The Cobalt Authors. All Rights Reserved.
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
{
  'includes': [
    'starboard_platform.gypi'
  ],
  'targets': [
    {
      'target_name': 'starboard_platform',
      'type': 'static_library',
      'sources': ['<@(starboard_platform_sources)'],
      'include_dirs': [
        '/usr/include/directfb',
      ],
      'defines': [
        # This must be defined when building Starboard, and must not when
        # building Starboard client code.
        'STARBOARD_IMPLEMENTATION',
      ],
      'dependencies': [
        '<@(starboard_platform_dependencies)',
      ],
      'conditions': [
        ['has_cdm==1', {
          'dependencies': [
            '<(DEPTH)/starboard/linux/x64x11/widevine.gyp:wvcdm_static',
          ],
        }],
      ],
    },
  ],
}
