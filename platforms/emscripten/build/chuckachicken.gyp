{
  'targets': [
    {
      'target_name': 'chuckachicken',
      'type': 'none',
      'dependencies': [
        '<(DEPTH)/nanaka/build/nanaka.gyp:nanaka',
        '<(DEPTH)/hobo/build/hobo.gyp:hobo',
      ],
      'include_dirs': [
        '<(DEPTH)',
        '<(DEPTH)/nanaka',
      ],
      'sources': [
        '<(DEPTH)/nanaka/pi/posix/FileManagerImpl.cpp',
        '<(DEPTH)/nanaka/pi/posix/FileManagerImpl.h',
        '<(DEPTH)/platforms/emscripten/src/Application.cpp',
        '<(DEPTH)/platforms/emscripten/src/Application.h',
        '<(DEPTH)/platforms/emscripten/src/Main.cpp',
      ],
    },
  ],
}
