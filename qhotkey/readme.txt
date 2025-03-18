# How to use this build "thing"
1. You need to have conan and cmake installed and ready
2. Check any changes in the source library: https://github.com/Skycoder42/QHotkey.git, this version works with the current master (17. 3. 2025)
3. Open command line and delete previous qhotkey package version from cache: 'conan remove qhotkey -f'
4. Build debug version of the library: 'conan create .  -s build_type=Debug' and release version: 'conan create .  -s build_type=Release'
5. Now you can use qhotkey as source package for your conan projects. Just add "qhotkey/1.4.2" into conanfile.py requires.