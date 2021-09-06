pkgname="gum"
pkgver="0.0.2"
pkgrel="1"
pkgdesc="GUMBO editor"
arch=("x86_64")
source=("https://github.com/fivanusec/GUMBO-Editor/archive/refs/heads/master.zip")
sha512sums=("SKIP")

build() {
    mkdir master
    unzip master -d master
    cd master/GUMBO-Editor-master
    bash build.sh
}

package() {
    cd master/GUMBO-Editor-master
    sudo cp build/src/gum /usr/bin/
}
