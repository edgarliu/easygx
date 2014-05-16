
install_path="install"

export PKG_CONFIG_PATH=$PWD/$install_path/lib/pkgconfig
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/$install_path/bin
export PATH=$PATH:$PWD/$install_path/bin
