project('Notepad', 'cpp', default_options: ['cpp_std=c++11'])

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Gui', 'Widgets'])
srcs = qt5.preprocess(moc_headers: ['mainwindow.hpp'])
executable('notepad', ['main.cpp', srcs], dependencies: [qt5_dep])
