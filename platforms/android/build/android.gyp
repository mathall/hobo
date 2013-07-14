{
  'targets': [
    {
      'target_name': 'android',
      'type': 'none',
      'dependencies': [
        '<(DEPTH)/nanaka/build/nanaka.gyp:nanaka',
        '<(DEPTH)/hobo/build/hobo.gyp:hobo',
      ],
    },
  ],
}
