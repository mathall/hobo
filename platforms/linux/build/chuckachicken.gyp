{
  'targets': [
    {
      'target_name': 'chuckachicken',
      'type': 'executable',
      'dependencies': [
        '<(DEPTH)/nanaka/build/nanaka.gyp:nanaka',
        '<(DEPTH)/hobo/build/hobo.gyp:hobo',
      ],
      'include_dirs': [
        '<(DEPTH)/platforms/linux/src',
        '<(DEPTH)',
        '<(DEPTH)/nanaka',
      ],
      'sources': [
        '<(DEPTH)/nanaka/pi/glfw/GLContextManagerImpl.cpp',
        '<(DEPTH)/nanaka/pi/glfw/GLContextManagerImpl.h',
        '<(DEPTH)/nanaka/pi/glfw/NanakaNativeWindowImpl.h',
        '<(DEPTH)/nanaka/pi/posix/FileManagerImpl.cpp',
        '<(DEPTH)/nanaka/pi/posix/FileManagerImpl.h',
        '<(DEPTH)/platforms/linux/src/Application.cpp',
        '<(DEPTH)/platforms/linux/src/Application.h',
        '<(DEPTH)/platforms/linux/src/Main.cpp',
      ],
      'link_settings': {
        'libraries': [
          '-lglfw',
          '-lGLEW',
        ],
      },
    },
  ],
}
