{
  'targets': [
    {
      'target_name': 'hobo',
      'type': '<(library_type)',
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
