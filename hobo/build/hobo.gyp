{
  'targets': [
    {
      'target_name': 'hobo',
      'type': 'static_library',
      'include_dirs': [
        '../..',
        '../../hobo',
        '../../nanaka',
      ],
      'includes': [
        '../main/main.gypi',
        '../gui/gui.gypi',
      ],
    },
  ],
}
