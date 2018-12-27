CTL_DIR=${CURDIR}
export CTL_DIR
#export DIR_RSDK=/home/frank/Project/WECB/rtl819x/toolchain/rsdk-1.3.6-5281-EB-2.6.30-0.9.30

revj:
	./target/revj/config_revj.sh
	make -C libs 
	make -C apps
	cd ./target/revj/ && ./install.sh

x86:
	./target/x86/config_x86.sh
	make -C libs 
	make -C apps
	cd ./target/x86/ && ./install.sh

wecb:
	./target/wecb/config_wecb.sh
	make -C libs 
	make -C apps
	cd ./target/wecb/ && ./install.sh

clean:
# make clean PLAT=xxx
# e.g. make clean PLAT=revj
ifeq ($(PLAT),)
	@echo " "
	@echo "        Please use make clean like this :"
	@echo " "
	@echo "                \"make clean PLAT=yourplatform\""
	@echo " "
	@echo "                 e.g. make clean PLAT=revj"
	@echo " "
	@echo "        NOTE: \"yourplatform\" are all in lowercase."
	@echo " "
else
	./target/${PLAT}/config_${PLAT}.sh
	make -C libs clean 
	make -C apps clean 
	rm -f global.def
endif
