# Contributer: Nicolas Dufour <capnemo@namshub-linux.org>
# For Archlinux
pkgname=gpar2
pkgver=0.2
pkgrel=1
pkgdesc="Parity Archive Volume Set"
url="http://parchive.sourceforge.net/"
license="GPL"
depends=( 'libpar2' 'gtkmm' )
makedepends=()
conflicts=()
replaces=()
backup=()
install=
source=(http://dl.sourceforge.net/sourceforge/parchive/$pkgname-$pkgver.tar.gz)
md5sums=('8f5bb2af3cee55e9574badd67d11a06e')

build() {
  cd $startdir/src/$pkgname-$pkgver
  ./configure --prefix=/usr
  make || return 1
  make DESTDIR=$startdir/pkg install

  find $startdir/pkg -name '*.la' -type f -exec rm {} \;
}
