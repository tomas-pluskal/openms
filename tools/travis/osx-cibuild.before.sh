#!/bin/bash

function build_contrib {
  cmake . -DBUILD_TYPE=$1

  if [ $? -ne 0 ]; then
    # we give it another try
    echo "1st attempt to build $1 failed .. retry"
    cmake . -DBUILD_TYPE=$1 -DNUMBER_OF_JOBS=4

    if [ $? -ne 0 ]; then
      echo "2nd attempt to build $1 failed .. abort"
      exit $?
    fi
  fi
}


# tap the science tap
brew tap homebrew/science
brew tap homebrew/versions
brew update
brew install libsvm xerces-c glpk coinmp
brew install qt4

# fetch contrib
git clone git://github.com/OpenMS/contrib/
pushd contrib

# we build seqan as the versions shipped in Ubuntu are not recent enough
build_contrib SEQAN

# we build WildMagic
build_contrib WILDMAGIC

# we build Eigen as the versions shipped in Ubuntu are not recent enough
build_contrib EIGEN

# leave contrib
popd

# regular builds .. get the search engine executables. don't fail if sourceforge is down ('|| true')
svn checkout http://svn.code.sf.net/p/open-ms/code/THIRDPARTY/Linux/64bit/ _thirdparty || true
# remove .svn otherwise we can't check out the other search engines into the same directory (TODO: maybe switch to wget)
rm -rf _thirdparty/.svn || true
svn checkout http://svn.code.sf.net/p/open-ms/code/THIRDPARTY/All/ _thirdparty || true
