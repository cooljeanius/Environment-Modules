# start .profile
if [ -f /etc/profile.modules ]
then
	. /etc/profile.modules
# put any module loads here
	module add null
fi

PATH=$PATH:$HOME/bin; export PATH
MANPATH=$MANPATH:$HOME/bin; export MANPATH

sh() { bash "$@"; }

# end .profile
