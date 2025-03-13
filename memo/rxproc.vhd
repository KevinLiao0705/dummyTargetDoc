

LIBRARY ieee;
USE ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;


--  Entity Declaration

ENTITY rxproc IS
	-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!
	PORT
	(
        reset_n	    	: 	in  STD_LOGIC;
        clksys 	    	: 	in  STD_LOGIC;
        rxin_f	    	: 	in  STD_LOGIC;
        rtime_cnt		: 	in  STD_LOGIC_VECTOR(15 downto 0);	--real time clock
		vgrx_tim_off	:	in  STD_LOGIC_VECTOR(15 downto 0);	--edge sync offset
		vgrt_tim_off	:	in  STD_LOGIC_VECTOR(15 downto 0);	--time sync offset
		rxid			:	in  STD_LOGIC_VECTOR(6 downto 0);
		------------------------------------------------------		
        rxclk_out		: 	out STD_LOGIC;						--rxclk,debug use
		------------------------------------------------------		
		vgrx_tim		:	out STD_LOGIC_VECTOR(15 downto 0);	--edge sync vgout time 
		vgrt_tim		:	out STD_LOGIC_VECTOR(15 downto 0);	--time sync vgout time
		vgout_en_tim_o	:	out std_logic_Vector(15 downto 0);	--vgout_enable_tim  
        vgout_en		:	out STD_LOGIC;						--vgout_en	
		------------------------------------------------------        
        rxpack_out		: 	out STD_LOGIC;						
        rx_data0		: 	out STD_LOGIC_VECTOR(15 downto 0);
        rx_data1		: 	out STD_LOGIC_VECTOR(15 downto 0);
        rx_data2		: 	out STD_LOGIC_VECTOR(15 downto 0);
        rx_data3		: 	out STD_LOGIC_VECTOR(15 downto 0);
        rx_data4		: 	out STD_LOGIC_VECTOR(15 downto 0)
	);


END rxproc;



ARCHITECTURE rxproc_body OF rxproc IS



SIGNAL rxpack_tim	:std_logic_Vector(15 downto 0);
SIGNAL rxclk_tim	:std_logic_Vector(4 downto 0);
SIGNAL rxpack_out_f	:std_logic;


SIGNAL rxbuf0:std_logic_Vector(15 downto 0);
SIGNAL rxbuf1:std_logic_Vector(15 downto 0);
SIGNAL rxbuf2:std_logic_Vector(15 downto 0);
SIGNAL rxbuf3:std_logic_Vector(15 downto 0);
SIGNAL rxbuf4:std_logic_Vector(15 downto 0);
SIGNAL rxbuf5:std_logic_Vector(15 downto 0);
SIGNAL rxbuf6:std_logic_Vector(15 downto 0);
SIGNAL rxbuf7:std_logic_Vector(15 downto 0);
SIGNAL rxbuf8:std_logic_Vector(15 downto 0);
SIGNAL rxbuf9:std_logic_Vector(15 downto 0);
SIGNAL rxbuf10:std_logic_Vector(15 downto 0);
SIGNAL rxbuf11:std_logic_Vector(15 downto 0);


SIGNAL rxd0:std_logic_Vector(15 downto 0);
SIGNAL rxd1:std_logic_Vector(15 downto 0);
SIGNAL rxd2:std_logic_Vector(15 downto 0);
SIGNAL rxd3:std_logic_Vector(15 downto 0);
SIGNAL rxd4:std_logic_Vector(15 downto 0);
SIGNAL rxd5:std_logic_Vector(15 downto 0);
SIGNAL rxd6:std_logic_Vector(15 downto 0);
SIGNAL rxd7:std_logic_Vector(15 downto 0);
SIGNAL rxd8:std_logic_Vector(15 downto 0);
SIGNAL rxd9:std_logic_Vector(15 downto 0);
SIGNAL rxd10:std_logic_Vector(15 downto 0);
SIGNAL rxd11:std_logic_Vector(15 downto 0);

SIGNAL rxchk:std_logic_Vector(15 downto 0);


SIGNAL vgrx_tim_offbuf:std_logic_Vector(15 downto 0);




SIGNAL vgout_en_tim:std_logic_Vector(15 downto 0);	
SIGNAL rx4m_cnt:std_logic_Vector(4 downto 0);	
SIGNAL rxin_cnt:std_logic_Vector(3 downto 0);	
SIGNAL rxclk_f:std_logic;	
SIGNAL vgout_en_f:std_logic;	

begin




	PROCESS(clksys)
  	BEGIN
  		if(rising_edge(clksys))then
			if(vgout_en_tim<3999)then		--50us time
				vgout_en_tim<=vgout_en_tim+1;
			else
				vgout_en_f<='0';
			end if;	
			
			if(rxpack_tim<639)then
				rxpack_tim<=rxpack_tim+1;
			else
				rxpack_out_f<='0';
			end if;	
			
			if(rxclk_f='0')then
				rxclk_tim<="00000";
			else	
				if(rxclk_tim<20)then
					rxclk_tim<=rxclk_tim+1;
				end if;	
				
				if(rxclk_tim="00000")then
					if(rxd0/=rxd6)then
						rxclk_tim<="10100";
					end if;
					if(rxd1/=rxd7)then
						rxclk_tim<="10100";
					end if;
					if(rxd2/=rxd8)then
						rxclk_tim<="10100";
					end if;
					if(rxd3/=rxd9)then
						rxclk_tim<="10100";
					end if;
					if(rxd4/=rxd10)then
						rxclk_tim<="10100";
					end if;
					if(rxd5/=rxd11)then
						rxclk_tim<="10100";
					end if;
					rxchk<=rxd0+rxd1;
				end if;
			
				if(rxclk_tim="00010")then
					rxchk<=rxchk+rxd2;
				end if;	
			
				if(rxclk_tim="00100")then
					rxchk<=rxchk+rxd3;
				end if;	
				
				if(rxclk_tim="00110")then
					rxchk<=rxchk+rxd4;
					vgrx_tim_offbuf<=vgrx_tim_off-rxd0(7 downto 0);
				end if;	
				if(rxclk_tim="01000")then
					if(rxchk=rxd5)then
						if(rxd0(15 downto 9)=rxid)then
							rxpack_out_f<='1';
							rxpack_tim<="0000000000000000";
							rx_data0<=rxd0;
							rx_data1<=rxd1;
							rx_data2<=rxd2;
							rx_data3<=rxd3;
							rx_data4<=rxd4;
							if(rxd0(8)='0')then
								vgrx_tim<=rtime_cnt+vgrx_tim_offbuf;
								vgrt_tim<=rxd4+vgrt_tim_off;
								vgout_en_f<='1';
								vgout_en_tim<="0000000000000000";
							else
								vgout_en_f<='0';
								vgout_en_tim<="0000000000000000";
							end if;	
						end if;
-----------------------------------------------------	
					end if;	
				end if;	
			end if;	
		end if;
	end process;











--Arxdata receiver 
-----------------------------------------------------	
	process(rxclk_f)
	begin
		if(rising_edge(rxclk_f))then 
			rxbuf0<=rxbuf0(14 downto 0)& rxbuf1(15);
			rxbuf1<=rxbuf1(14 downto 0)& rxbuf2(15);
			rxbuf2<=rxbuf2(14 downto 0)& rxbuf3(15);
			rxbuf3<=rxbuf3(14 downto 0)& rxbuf4(15);
			rxbuf4<=rxbuf4(14 downto 0)& rxbuf5(15);
			rxbuf5<=rxbuf5(14 downto 0)& rxbuf6(15);
			rxbuf6<=rxbuf6(14 downto 0)& rxbuf7(15);
			rxbuf7<=rxbuf7(14 downto 0)& rxbuf8(15);
			rxbuf8<=rxbuf8(14 downto 0)& rxbuf9(15);
			rxbuf9<=rxbuf9(14 downto 0)& rxbuf10(15);
			rxbuf10<=rxbuf10(14 downto 0)& rxbuf11(15);
			rxbuf11<=rxbuf11(14 downto 0)& rxin_f;
			
			rxd0(15 downto 12)<=rxbuf0(15)& rxbuf0(13)& rxbuf0(11)& rxbuf0(9);
			rxd0(11 downto 8) <=rxbuf0(7) & rxbuf0(5) & rxbuf0(3) & rxbuf0(1);
			rxd0(7 downto 4)  <=rxbuf1(15)& rxbuf1(13)& rxbuf1(11)& rxbuf1(9);
			rxd0(3 downto 0)  <=rxbuf1(7) & rxbuf1(5) & rxbuf1(3) & rxbuf1(1);
			
			rxd1(15 downto 12)<=rxbuf2(15)& rxbuf2(13)& rxbuf2(11)& rxbuf2(9);
			rxd1(11 downto 8) <=rxbuf2(7) & rxbuf2(5) & rxbuf2(3) & rxbuf2(1);
			rxd1(7 downto 4)  <=rxbuf3(15)& rxbuf3(13)& rxbuf3(11)& rxbuf3(9);
			rxd1(3 downto 0)  <=rxbuf3(7) & rxbuf3(5) & rxbuf3(3) & rxbuf3(1);
			
			rxd2(15 downto 12)<=rxbuf4(15)& rxbuf4(13)& rxbuf4(11)& rxbuf4(9);
			rxd2(11 downto 8) <=rxbuf4(7) & rxbuf4(5) & rxbuf4(3) & rxbuf4(1);
			rxd2(7 downto 4)  <=rxbuf5(15)& rxbuf5(13)& rxbuf5(11)& rxbuf5(9);
			rxd2(3 downto 0)  <=rxbuf5(7) & rxbuf5(5) & rxbuf5(3) & rxbuf5(1);
			
			rxd3(15 downto 12)<=rxbuf6(15)& rxbuf6(13)& rxbuf6(11)& rxbuf6(9);
			rxd3(11 downto 8) <=rxbuf6(7) & rxbuf6(5) & rxbuf6(3) & rxbuf6(1);
			rxd3(7 downto 4)  <=rxbuf7(15)& rxbuf7(13)& rxbuf7(11)& rxbuf7(9);
			rxd3(3 downto 0)  <=rxbuf7(7) & rxbuf7(5) & rxbuf7(3) & rxbuf7(1);
			
			rxd4(15 downto 12)<=rxbuf8(15)& rxbuf8(13)& rxbuf8(11)& rxbuf8(9);
			rxd4(11 downto 8) <=rxbuf8(7) & rxbuf8(5) & rxbuf8(3) & rxbuf8(1);
			rxd4(7 downto 4)  <=rxbuf9(15)& rxbuf9(13)& rxbuf9(11)& rxbuf9(9);
			rxd4(3 downto 0)  <=rxbuf9(7) & rxbuf9(5) & rxbuf9(3) & rxbuf9(1);
			
			rxd5(15 downto 12)<=rxbuf10(15)& rxbuf10(13)& rxbuf10(11)& rxbuf10(9);
			rxd5(11 downto 8) <=rxbuf10(7) & rxbuf10(5) & rxbuf10(3) & rxbuf10(1);
			rxd5(7 downto 4)  <=rxbuf11(15)& rxbuf11(13)& rxbuf11(11)& rxbuf11(9);
			rxd5(3 downto 0)  <=rxbuf11(7) & rxbuf11(5) & rxbuf11(3) & rxbuf11(1);
			
			
			
			rxd6(15 downto 12)<=not rxbuf0(14) &not rxbuf0(12)&not rxbuf0(10)&not rxbuf0(8);
			rxd6(11 downto 8) <=not rxbuf0(6)  &not rxbuf0(4) &not rxbuf0(2) &not rxbuf0(0);
			rxd6(7 downto 4)  <=not rxbuf1(14) &not rxbuf1(12)&not rxbuf1(10)&not rxbuf1(8);
			rxd6(3 downto 0)  <=not rxbuf1(6)  &not rxbuf1(4) &not rxbuf1(2) &not rxbuf1(0);
			
			rxd7(15 downto 12)<=not rxbuf2(14) &not rxbuf2(12)&not rxbuf2(10)&not rxbuf2(8);
			rxd7(11 downto 8) <=not rxbuf2(6)  &not rxbuf2(4) &not rxbuf2(2) &not rxbuf2(0);
			rxd7(7 downto 4)  <=not rxbuf3(14) &not rxbuf3(12)&not rxbuf3(10)&not rxbuf3(8);
			rxd7(3 downto 0)  <=not rxbuf3(6)  &not rxbuf3(4) &not rxbuf3(2) &not rxbuf3(0);
			
			rxd8(15 downto 12)<=not rxbuf4(14) &not rxbuf4(12)&not rxbuf4(10)&not rxbuf4(8);
			rxd8(11 downto 8) <=not rxbuf4(6)  &not rxbuf4(4) &not rxbuf4(2) &not rxbuf4(0);
			rxd8(7 downto 4)  <=not rxbuf5(14) &not rxbuf5(12)&not rxbuf5(10)&not rxbuf5(8);
			rxd8(3 downto 0)  <=not rxbuf5(6)  &not rxbuf5(4) &not rxbuf5(2) &not rxbuf5(0);
			
			rxd9(15 downto 12)<=not rxbuf6(14) &not rxbuf6(12)&not rxbuf6(10)&not rxbuf6(8);
			rxd9(11 downto 8) <=not rxbuf6(6)  &not rxbuf6(4) &not rxbuf6(2) &not rxbuf6(0);
			rxd9(7 downto 4)  <=not rxbuf7(14) &not rxbuf7(12)&not rxbuf7(10)&not rxbuf7(8);
			rxd9(3 downto 0)  <=not rxbuf7(6)  &not rxbuf7(4) &not rxbuf7(2) &not rxbuf7(0);
			
			rxd10(15 downto 12)<=not rxbuf8(14) &not rxbuf8(12)&not rxbuf8(10)&not rxbuf8(8);
			rxd10(11 downto 8) <=not rxbuf8(6)  &not rxbuf8(4) &not rxbuf8(2) &not rxbuf8(0);
			rxd10(7 downto 4)  <=not rxbuf9(14) &not rxbuf9(12)&not rxbuf9(10)&not rxbuf9(8);
			rxd10(3 downto 0)  <=not rxbuf9(6)  &not rxbuf9(4) &not rxbuf9(2) &not rxbuf9(0);
			
			rxd11(15 downto 12)<=not rxbuf10(14) &not rxbuf10(12)&not rxbuf10(10)&not rxbuf10(8);
			rxd11(11 downto 8) <=not rxbuf10(6)  &not rxbuf10(4) &not rxbuf10(2) &not rxbuf10(0);
			rxd11(7 downto 4)  <=not rxbuf11(14) &not rxbuf11(12)&not rxbuf11(10)&not rxbuf11(8);
			rxd11(3 downto 0)  <=not rxbuf11(6)  &not rxbuf11(4) &not rxbuf11(2) &not rxbuf11(0);
			
			
			
		end if;	
	end process;			



--rxclk generator
-----------------------------------------------------	
	PROCESS(clksys)
  	BEGIN
		if(rising_edge(clksys))then 
			if(rx4m_cnt="10011")then
				rx4m_cnt<="00000";
			else
				rx4m_cnt<=rx4m_cnt+1;
			end if;
			
			if(rx4m_cnt<8)then
				rxclk_f<='0';
			elsif(rx4m_cnt<18)then
				rxclk_f<='1';
			else
				rxclk_f<='0';
			end if;
			
			if(rxin_f='0')then 
				rxin_cnt<="0000";
			else
				if(rxin_cnt(3)='0')then
					rxin_cnt<=rxin_cnt+1;
					if(rxin_cnt="0010")then
						case rx4m_cnt is
							when "00000"=>
								rx4m_cnt<="00001";			
							when "00001"=>
								rx4m_cnt<="00001";			
							when "00010"=>
								rx4m_cnt<="00010";			
							when "00011"=>
								rx4m_cnt<="00010";			
							when "00100"=>
								rx4m_cnt<="00100";			
							when "00101"=>
								rx4m_cnt<="00101";			
							when "00110"=>
								rx4m_cnt<="00110";			
							when "00111"=>
								rx4m_cnt<="00111";			
							when "01000"=>
								rx4m_cnt<="01000";			
							when "01001"=>
								rx4m_cnt<="01001";			
							when "01010"=>
								rx4m_cnt<="01010";			
							when "01011"=>
								rx4m_cnt<="01101";			
							when "01100"=>
								rx4m_cnt<="01110";			
							when "01101"=>
								rx4m_cnt<="01111";			
							when "01110"=>
								rx4m_cnt<="10000";			
							when "01111"=>
								rx4m_cnt<="10001";			
							when "10000"=>
								rx4m_cnt<="10010";			
							when "10001"=>
								rx4m_cnt<="10011";			
							when "10010"=>
								rx4m_cnt<="00000";			
							when "10011"=>
								rx4m_cnt<="00001";			
							when others =>
						end case;
					end if;
				end if;
			end if;
		end if;
	end process;	
-----------------------------------------------------	
	rxclk_out<=rxclk_f;
	rxpack_out<=rxpack_out_f;
	vgout_en<=vgout_en_f;
	vgout_en_tim_o<=vgout_en_tim;

























-----------------------------------------------------	

END rxproc_body;
