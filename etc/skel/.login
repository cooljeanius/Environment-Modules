# start .login
if ( -e /etc/csh.modules ) then
	source /etc/csh.modules
# put any module loads here
	module add null
endif

# Interative Shell variables and settings
set path=($path ~/bin)
setenv MANPATH $MANPATH":"$HOME/man

if (! $?prompt) exit			#exit if not interactive

# end .login
