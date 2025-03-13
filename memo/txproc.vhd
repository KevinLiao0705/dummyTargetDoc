

LIBRARY ieee;
USE ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

--  Entity Declaration

ENTITY txproc IS
	-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!
	PORT
	(
        reset_n	    	: 	in  STD_LOGIC;
        clksys 	    	: 	in  STD_LOGIC;
        video_gate  	: 	in  STD_LOGIC;
        txcon_f		  	: 	in  STD_LOGIC;
        tx_data0    	: 	in  STD_LOGIC_VECTOR(15 downto 0);
        tx_data1    	: 	in  STD_LOGIC_VECTOR(15 downto 0);
        tx_data2    	: 	in  STD_LOGIC_VECTOR(15 downto 0);
        tx_data3    	: 	in  STD_LOGIC_VECTOR(15 downto 0);
        tx_data4    	: 	in  STD_LOGIC_VECTOR(15 downto 0);
        txout_f			: 	out  STD_LOGIC;				
        clk4m_out		: 	out  STD_LOGIC;				
        rfen_f		    :   in  STD_LOGIC;
        syncin_f	    :   in  STD_LOGIC
	);


END txproc;



ARCHITECTURE txproc_body OF txproc IS

--4M txclk generator
--input clksys,video_gate
--output clk4m,txload_f
SIGNAL clk4m_cnt:std_logic_Vector(4 downto 0);	
SIGNAL txbit_cnt:std_logic_Vector(9 downto 0);
SIGNAL txload_f:std_logic;
SIGNAL clk4m_f:std_logic;
SIGNAL sync4m_cnt:std_logic_Vector(4 downto 0);	
SIGNAL clk4mh_cnt:std_logic_Vector(3 downto 0);	
SIGNAL clk4ml_cnt:std_logic_Vector(3 downto 0);	
SIGNAL wait_cnt:std_logic_Vector(3 downto 0);	
SIGNAL synctx_tim:std_logic_Vector(7 downto 0);	
SIGNAL sync4m_f:std_logic;
SIGNAL syncin_cnt:std_logic_Vector(3 downto 0);	


--txdata dispatch 
--input clysys,txload,reset_n,Adata2-0,Atime,vg_tim_off;
--output vgout_en_f,txbuf0-13,vg_tim
SIGNAL vgout_en_tim:std_logic_Vector(15 downto 0);	
SIGNAL txload_cnt:std_logic_Vector(3 downto 0);	
SIGNAL txd5:std_logic_Vector(15 downto 0);	
SIGNAL txd4:std_logic_Vector(15 downto 0);	
SIGNAL txd3:std_logic_Vector(15 downto 0);	
SIGNAL txd2:std_logic_Vector(15 downto 0);	
SIGNAL txd1:std_logic_Vector(15 downto 0);	
SIGNAL txd0:std_logic_Vector(15 downto 0);	
SIGNAL txbuf0:std_logic_Vector(15 downto 0);
SIGNAL txbuf1:std_logic_Vector(15 downto 0);
SIGNAL txbuf2:std_logic_Vector(15 downto 0);
SIGNAL txbuf3:std_logic_Vector(15 downto 0);
SIGNAL txbuf4:std_logic_Vector(15 downto 0);
SIGNAL txbuf5:std_logic_Vector(15 downto 0);
SIGNAL txbuf6:std_logic_Vector(15 downto 0);
SIGNAL txbuf7:std_logic_Vector(15 downto 0);
SIGNAL txbuf8:std_logic_Vector(15 downto 0);
SIGNAL txbuf9:std_logic_Vector(15 downto 0);
SIGNAL txbuf10:std_logic_Vector(15 downto 0);
SIGNAL txbuf11:std_logic_Vector(15 downto 0);
SIGNAL txbuf12:std_logic_Vector(15 downto 0);
SIGNAL txbuf13:std_logic_Vector(15 downto 0);
SIGNAL txbuf14:std_logic_Vector(15 downto 0);
SIGNAL txbuf15:std_logic_Vector(15 downto 0);

--txdata transmit 	
--input txbuf0-13,clk4m,txload_f,reset_n
--output txout_f
SIGNAL txbuf0b:std_logic_Vector(15 downto 0);
SIGNAL txbuf1b:std_logic_Vector(15 downto 0);
SIGNAL txbuf2b:std_logic_Vector(15 downto 0);
SIGNAL txbuf3b:std_logic_Vector(15 downto 0);
SIGNAL txbuf4b:std_logic_Vector(15 downto 0);
SIGNAL txbuf5b:std_logic_Vector(15 downto 0);
SIGNAL txbuf6b:std_logic_Vector(15 downto 0);
SIGNAL txbuf7b:std_logic_Vector(15 downto 0);
SIGNAL txbuf8b:std_logic_Vector(15 downto 0);
SIGNAL txbuf9b:std_logic_Vector(15 downto 0);
SIGNAL txbuf10b:std_logic_Vector(15 downto 0);
SIGNAL txbuf11b:std_logic_Vector(15 downto 0);
SIGNAL txbuf12b:std_logic_Vector(15 downto 0);
SIGNAL txbuf13b:std_logic_Vector(15 downto 0);
SIGNAL txbuf14b:std_logic_Vector(15 downto 0);
SIGNAL txbuf15b:std_logic_Vector(15 downto 0);

















begin





--sync4m generator
-----------------------------------------------------	
	PROCESS(clksys)
  	BEGIN
		if(rising_edge(clksys))then 
			if(sync4m_cnt="10011")then
				sync4m_cnt<="00000";
			else
				sync4m_cnt<=sync4m_cnt+1;
			end if;
			
			if(sync4m_cnt<8)then
				sync4m_f<='0';
			elsif(sync4m_cnt<18)then
				sync4m_f<='1';
			else
				sync4m_f<='0';
			end if;
			
			if(rfen_f='1')then
				if(syncin_f='0')then 
					syncin_cnt<="0000";
				else
					if(syncin_cnt(3)='0')then
						syncin_cnt<=syncin_cnt+1;
						if(syncin_cnt="0010")then
							case sync4m_cnt is
								when "00000"=>
									sync4m_cnt<="00001";	--no chg		
								when "00001"=>
									sync4m_cnt<="00001";			
								when "00010"=>
									sync4m_cnt<="00010";			
								when "00011"=>
									sync4m_cnt<="00010";			
								when "00100"=>
									sync4m_cnt<="00100";			
								when "00101"=>
									sync4m_cnt<="00101";			
								when "00110"=>
									sync4m_cnt<="00110";			
								when "00111"=>
									sync4m_cnt<="00111";			
								when "01000"=>
									sync4m_cnt<="01000";			
								when "01001"=>
									sync4m_cnt<="01001";			
								when "01010"=>
									sync4m_cnt<="01010";			
								when "01011"=>
									sync4m_cnt<="01101";			
								when "01100"=>
									sync4m_cnt<="01110";			
								when "01101"=>
									sync4m_cnt<="01111";			
								when "01110"=>
									sync4m_cnt<="10000";			
								when "01111"=>
									sync4m_cnt<="10001";			
								when "10000"=>
									sync4m_cnt<="10010";			
								when "10001"=>
									sync4m_cnt<="10011";			
								when "10010"=>
									sync4m_cnt<="00000";			
								when "10011"=>
									sync4m_cnt<="00001";			
								when others =>
							end case;
						end if;
					end if;
				end if;
			end if;
		end if;
	end process;	
-----------------------------------------------------	





--4M txclk generator
--input clksys,video_gate
--output clk4m,txload_f
-----------------------------------------------------	
	PROCESS(clksys,video_gate,reset_n)
  	BEGIN
		if(reset_n='0')then
			txload_f<='0';	
			clk4m_f<='0';
			clk4mh_cnt<="0000";
			clk4ml_cnt<="0000";
		else
			if(rising_edge(clksys))then 
				----------------------------	
				if(video_gate='1')then
					txbit_cnt<="0000000000";
					wait_cnt<="0000";
					clk4mh_cnt<="0000";
					clk4ml_cnt<="0000";
					txload_f<='0';	
					clk4m_f<='0';
					synctx_tim<="00000000";
				else	
					synctx_tim<=synctx_tim+1;
					if(sync4m_f='0')then
						clk4mh_cnt<="0000";
						if(clk4ml_cnt<10)then
							clk4ml_cnt<=clk4ml_cnt+1;
						end if;	
						if(clk4ml_cnt="0100")then
							txload_f<='0';
							clk4m_f<='0';
						end if;
					else
						clk4ml_cnt<="0000";
						if(clk4mh_cnt<10)then
							clk4mh_cnt<=clk4mh_cnt+1;
						end if;
						if(clk4mh_cnt="0100")then
							if(wait_cnt<10)then
								wait_cnt<=wait_cnt+1;
							end if;	
							if(wait_cnt=3)then
								txload_f<='1';
							end if;	
							if(wait_cnt>=4)then
								if(txbit_cnt < 248 )then--256
									if(txcon_f='0')then
										txbit_cnt<=txbit_cnt+1;	
									end if;	
									clk4m_f<='1';
								end if;	
							end if;	
						end if;
					end if;
				end if;	
			end if;	
		end if;	
	end process;
-----------------------------------------------------	











--txdata dispatch 
--input clysys,txload,reset_n,Adata2-0,Atime,vg_tim_off;
--output vgout_en_f,txbuf0-13,vg_tim
-----------------------------------------------------	
	PROCESS(clksys,txload_f,reset_n)
  	BEGIN
		if(reset_n='0')then
		else
			if(rising_edge(clksys))then 
				if(txload_f='0')then
					txload_cnt<="0000";
					txd5<=tx_data0;
					txd5(15 downto 8)<=tx_data0(15 downto 8);
					txd5(7 downto 0)<=synctx_tim;
					txd4<=tx_data1;
					txd3<=tx_data2;
					txd2<=tx_data3;
					txd1<=tx_data4;
					txd0<=tx_data3+tx_data4;
				else	
					if(txload_cnt<10)then
						txload_cnt<=txload_cnt+1;
					end if;	
					--------------------------------
					if(txload_cnt="0010")then
						txd0<=txd0+txd3;
					end if;
					--------------------------------
					if(txload_cnt="0100")then
						txd0<=txd0+txd4;
					end if;
					--------------------------------
					if(txload_cnt="0110")then
						txd0<=txd0+txd5;
					end if;
					--------------------------------
					if(txload_cnt="1000")then
						txbuf0(15 downto 0)<="0101010101010101";
						txbuf1(15 downto 0)<="0101010101010101";
						txbuf2(15 downto 0)<="0101010101010101";
						txbuf3(15 downto 0)<="0101010101010101";
						----------------------------------------------------------------------------------------------------------------------
						txbuf4(15 downto 8)<=txd5(15)&not txd5(15)&txd5(14)&not txd5(14)&txd5(13)&not txd5(13)&txd5(12)&not txd5(12);
						txbuf4(7 downto 0)<= txd5(11)&not txd5(11)&txd5(10)&not txd5(10)&txd5(9) &not txd5(9) &txd5(8) &not txd5(8);
						txbuf5(15 downto 8)<=txd5(7) &not txd5(7) &txd5(6) &not txd5(6) &txd5(5) &not txd5(5) &txd5(4) &not txd5(4);
						txbuf5(7 downto 0)<= txd5(3) &not txd5(3) &txd5(2) &not txd5(2) &txd5(1) &not txd5(1) &txd5(0) &not txd5(0);
						----------------------------------------------------------------------------------------------------------------------
						txbuf6(15 downto 8)<=txd4(15)&not txd4(15)&txd4(14)&not txd4(14)&txd4(13)&not txd4(13)&txd4(12)&not txd4(12);
						txbuf6(7 downto 0)<= txd4(11)&not txd4(11)&txd4(10)&not txd4(10)&txd4(9) &not txd4(9) &txd4(8) &not txd4(8);
						txbuf7(15 downto 8)<=txd4(7) &not txd4(7) &txd4(6) &not txd4(6) &txd4(5) &not txd4(5) &txd4(4) &not txd4(4);
						txbuf7(7 downto 0)<= txd4(3) &not txd4(3) &txd4(2) &not txd4(2) &txd4(1) &not txd4(1) &txd4(0) &not txd4(0);
						----------------------------------------------------------------------------------------------------------------------
						txbuf8(15 downto 8)<=txd3(15)&not txd3(15)&txd3(14)&not txd3(14)&txd3(13)&not txd3(13)&txd3(12)&not txd3(12);
						txbuf8(7 downto 0)<= txd3(11)&not txd3(11)&txd3(10)&not txd3(10)&txd3(9) &not txd3(9) &txd3(8) &not txd3(8);
						txbuf9(15 downto 8)<=txd3(7) &not txd3(7) &txd3(6) &not txd3(6) &txd3(5) &not txd3(5) &txd3(4) &not txd3(4);
						txbuf9(7 downto 0)<= txd3(3) &not txd3(3) &txd3(2) &not txd3(2) &txd3(1) &not txd3(1) &txd3(0) &not txd3(0);
						----------------------------------------------------------------------------------------------------------------------
						txbuf10(15 downto 8)<=txd2(15)&not txd2(15)&txd2(14)&not txd2(14)&txd2(13)&not txd2(13)&txd2(12)&not txd2(12);
						txbuf10(7 downto 0)<= txd2(11)&not txd2(11)&txd2(10)&not txd2(10)&txd2(9) &not txd2(9) &txd2(8) &not txd2(8);
						txbuf11(15 downto 8)<=txd2(7) &not txd2(7) &txd2(6) &not txd2(6) &txd2(5) &not txd2(5) &txd2(4) &not txd2(4);
						txbuf11(7 downto 0)<= txd2(3) &not txd2(3) &txd2(2) &not txd2(2) &txd2(1) &not txd2(1) &txd2(0) &not txd2(0);
						----------------------------------------------------------------------------------------------------------------------
						txbuf12(15 downto 8)<=txd1(15)&not txd1(15)&txd1(14)&not txd1(14)&txd1(13)&not txd1(13)&txd1(12)&not txd1(12);
						txbuf12(7 downto 0)<= txd1(11)&not txd1(11)&txd1(10)&not txd1(10)&txd1(9) &not txd1(9) &txd1(8) &not txd1(8);
						txbuf13(15 downto 8)<=txd1(7) &not txd1(7) &txd1(6) &not txd1(6) &txd1(5) &not txd1(5) &txd1(4) &not txd1(4);
						txbuf13(7 downto 0)<= txd1(3) &not txd1(3) &txd1(2) &not txd1(2) &txd1(1) &not txd1(1) &txd1(0) &not txd1(0);
						----------------------------------------------------------------------------------------------------------------------
						txbuf14(15 downto 8)<=txd0(15)&not txd0(15)&txd0(14)&not txd0(14)&txd0(13)&not txd0(13)&txd0(12)&not txd0(12);
						txbuf14(7 downto 0)<= txd0(11)&not txd0(11)&txd0(10)&not txd0(10)&txd0(9) &not txd0(9) &txd0(8) &not txd0(8);
						txbuf15(15 downto 8)<=txd0(7) &not txd0(7) &txd0(6) &not txd0(6) &txd0(5) &not txd0(5) &txd0(4) &not txd0(4);
						txbuf15(7 downto 0)<= txd0(3) &not txd0(3) &txd0(2) &not txd0(2) &txd0(1) &not txd0(1) &txd0(0) &not txd0(0);
						----------------------------------------------------------------------------------------------------------------------
					end if;
				end if;
			end if;
		end if;			
	end process;
-----------------------------------------------------	
	




--txdata transmit 	
--input txbuf0-13,clk4m,txload_f,reset_n
--output txout_f

-----------------------------------------------------	
	PROCESS(clk4m_f,txload_f,reset_n)
	
  	BEGIN
		if(reset_n='0')then
			txout_f<='0';
		else
			if(txload_f='1')then
				txbuf0b<=txbuf0;
				txbuf1b<=txbuf1;
				txbuf2b<=txbuf2;
				txbuf3b<=txbuf3;
				txbuf4b<=txbuf4;
				txbuf5b<=txbuf5;
				txbuf6b<=txbuf6;
				txbuf7b<=txbuf7;
				txbuf8b<=txbuf8;
				txbuf9b<=txbuf9;
				txbuf10b<=txbuf10;
				txbuf11b<=txbuf11;
				txbuf12b<=txbuf12;
				txbuf13b<=txbuf13;
				txbuf14b<=txbuf14;
				txbuf15b<=txbuf15;
				txout_f<='0';
			else	
				if(rising_edge(clk4m_f))then 
					txout_f<=txbuf0b(7);	--txbuf0b(15);	
					txbuf0b<= txbuf0b(14 downto 0) & txbuf1b(15);
					txbuf1b<= txbuf1b(14 downto 0) & txbuf2b(15);
					txbuf2b<= txbuf2b(14 downto 0) & txbuf3b(15);
					txbuf3b<= txbuf3b(14 downto 0) & txbuf4b(15);
					txbuf4b<= txbuf4b(14 downto 0) & txbuf5b(15);
					txbuf5b<= txbuf5b(14 downto 0) & txbuf6b(15);
					txbuf6b<= txbuf6b(14 downto 0) & txbuf7b(15);
					txbuf7b<= txbuf7b(14 downto 0) & txbuf8b(15);
					txbuf8b<= txbuf8b(14 downto 0) & txbuf9b(15);
					txbuf9b<= txbuf9b(14 downto 0) & txbuf10b(15);
					txbuf10b<=txbuf10b(14 downto 0)& txbuf11b(15);
					txbuf11b<=txbuf11b(14 downto 0)& txbuf12b(15);
					txbuf12b<=txbuf12b(14 downto 0)& txbuf13b(15);
					txbuf13b<=txbuf13b(14 downto 0)& txbuf14b(15);
					txbuf14b<=txbuf14b(14 downto 0)& txbuf15b(15);
					txbuf15b<=txbuf15b(14 downto 0)& not txbuf15b(0);
				end if;	
			end if;
		end if;	
	end process;
	
	clk4m_out<=clk4m_f;
--	clk4m_out<=sync4m_f;
-----------------------------------------------------	

END txproc_body;

