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
        '<(DEPTH)/platforms/osx/src',
        '<(DEPTH)',
        '<(DEPTH)/nanaka',
      ],
      'sources': [
        '<(DEPTH)/nanaka/pi/glfw/GLContextManagerImpl.cpp',
        '<(DEPTH)/nanaka/pi/glfw/GLContextManagerImpl.h',
        '<(DEPTH)/nanaka/pi/glfw/NanakaNativeWindowImpl.h',
        '<(DEPTH)/nanaka/pi/posix/FileManagerImpl.cpp',
        '<(DEPTH)/nanaka/pi/posix/FileManagerImpl.h',
        '<(DEPTH)/platforms/osx/src/Application.cpp',
        '<(DEPTH)/platforms/osx/src/Application.h',
        '<(DEPTH)/platforms/osx/src/Main.cpp',
      ],
      'link_settings': {
        'libraries': [
          '-lglfw3',
          '-lGLEW',
        ],
      },
    },
  ],
}
