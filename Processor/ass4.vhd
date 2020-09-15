----------------------------------------------------------------------------------
-- Company: IIT Delhi
-- Engineer: Manoj Kumar (github : manoj2601)
-- 
-- Create Date: 01/26/2020 03:17:12 PM
-- Design Name: 
-- Module Name: ass3 - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;



-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ass4 is
  Port (
    clk: IN std_logic;
    en2 : IN std_logic;
    an0 : OUT std_logic;
     an1 : OUT std_logic;
     an2 : OUT std_logic;
     an3 : OUT std_logic;
     push : IN std_logic;
     push2 : IN std_logic;
     r1 : IN std_logic;
     r2 : IN std_logic;
     r3 : IN std_logic;
     r4 : IN std_logic;
     r5 : IN std_logic;
     seg : OUT std_logic_vector(6 downto 0)
     );
end ass4;

architecture Behavioral of ass4 is

type reg is array(0 to 31) of std_logic_vector(31 downto 0);
signal reg1 : reg
    :=(("00000000000000000000000000000000"), 
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000111"), --a0
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),--6
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"), --t0
    ("00000000000000000000000000000011"), --t1
    ("00000000000000000000000000000000"), --t2
    ("00000000000000000000000000000010"), --t3
    ("00000000000000000000000000000001"), --t4
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),                                     
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),--23
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"),
    ("00000000000000000000111111111111"), --29 $sp
    ("00000000000000000000000000000000"),
    ("00000000000000000000000000000000"));
    
 
    
signal emptyvect : std_logic_vector(31 downto 0):="00000000000000000000000000000010";
signal rsrc : integer :=0;
signal rsrc1 : integer :=0;
signal rsrc2 : integer :=0;
signal rdest : integer :=0;
signal index : integer :=0;
signal samt : integer:=0;
signal en1 : std_logic := '0';
--signal shruti : integer :=1;
signal test : integer :=0;
TYPE state_type is (idle, read, updateir, shiftoper, shiftlefthelper, shiftrighthelper, change, s1, endstate , comparison, comparisonzero, jump,  jumpregister, jumplink  );
signal state : state_type :=idle;
signal temp : std_logic_vector(31 downto 0);
signal count: integer :=0;
signal rtype : integer := 0;
signal itype : integer :=0;
signal ifirst : integer := 0;
signal irs : integer := 0;
signal ird : integer :=0;
signal iaddress : integer := 0;
signal iaddress1 : integer :=0;
signal irs1 : integer := 0;
signal ird1 : integer := 0;
signal manoj : integer := 0;
signal shamt : integer := 0;
signal funct : integer := 0;
signal wea : std_logic_VECTOR(0 DOWNTO 0) :="0";
signal addra : std_logic_vector(11 DOWNTO 0) ;
signal dina : std_logic_vector(31 DOWNTO 0); 
signal douta : std_logic_vector(31 DOWNTO 0) ;
signal sevensegment1 : std_logic_vector(15 DOWNTO 0);
signal sevensegment2 : std_logic_vector(15 DOWNTO 0);
signal instruction:  std_logic_vector(31 DOWNTO 0);
signal finalresult : std_logic_vector(15 DOWNTO 0);
signal counter : integer := 0;
signal temp1 : integer := 0;
signal clk1 : std_logic := '0';

---------------------------------------------------------------------------------------------------------------
signal regfirst : integer := 0;
signal regsecond : integer := 0;
signal intaddress : integer := 0;
signal address_pc : std_logic_vector(15 DOWNTO 0);
signal sp : integer := 4095;
signal regindex : integer := 0;
signal regprint : std_logic_vector(4 downto 0);
signal regprintindex : integer := 0;
--------------------------------------------------------------------------------------------------------------------

component blk_mem_gen_0
PORT (
    clka : IN STD_LOGIC;
    wea : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
    addra: IN STD_LOGIC_VECTOR(11 DOWNTO 0);
    dina: IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    douta : out STD_LOGIC_VECTOR(31 DOWNTO 0)
);
end component;

component sevensegment
 Port (
 clk : IN std_logic;
 en1 : IN std_logic;
 an0 : OUT std_logic;
 an1 : OUT std_logic;
 an2 : OUT std_logic;
 an3 : OUT std_logic;
 bitset1: IN std_logic_vector(3 downto 0);
 bitset2: IN std_logic_vector(3 downto 0);
 bitset3: IN std_logic_vector(3 downto 0);
 bitset4: IN std_logic_vector(3 downto 0);
 seg : OUT std_logic_vector(6 downto 0)
  );
end component;

begin

U1 : blk_mem_gen_0
port map(
    clka => clk,
    wea => wea,
    dina => dina,
    addra => addra,
    douta => douta
    );     
    
U2 : sevensegment
    port map(
        clk => clk,
        en1 => en1,
        an0 => an0,
        an1 => an1,
        an2 => an2,
        an3 => an3,
        bitset1 => sevensegment2(3 downto 0),
        bitset2 => sevensegment2(7 downto 4),
        bitset3 => sevensegment2(11 downto 8),
        bitset4 => sevensegment2(15 downto 12),
        seg => seg
        );         


process(clk)
begin
    if(clk'event and clk = '1') then
        regprint(0) <= r1;
        regprint(1) <= r2;
        regprint(2) <= r3;
        regprint(3) <= r4;
        regprint(4) <= r5;
        regprintindex <= to_integer(unsigned(regprint));
        if(push2 = '1') then
        sevensegment1 <= reg1(to_integer(unsigned(regprint)))(15 downto 0);
        end if;
    end if;

end process;


process(clk)
    begin
        if(clk'event and clk = '1') then
        temp1 <= temp1+1;
        if(temp1 = 5000000 and push = '0') then
            clk1 <= not clk1;
            temp1 <= 0;
        end if;
      end if;
    end process;   


process(clk1, state, en2)
    begin
    if (clk1'EVENT and clk1 = '1') then
--        if(count<10) then
--           temp<=mem1(0);
           case state is
                when idle =>
                
                en1 <= '0';
                
                    itype<=0;
                    rtype<=0;
                    if(count<100) then
                        wea <= "0";
                        addra <= std_logic_vector(to_unsigned(count,12));
--                        temp<=mem1(count);
                        state <= read;
                        end if;                                       
                        
                       
                  
                when read =>
--                wea<="1";
                manoj<=0;
                    if (douta = "00000000000000000000000000000000") then
                                   state <= endstate;
                    
                    elsif(douta(31 downto 26)="000000" and douta(5 downto 0) /= "001000") then --R type
                    count <= count+1; 
                        rsrc1<=to_integer(unsigned(douta(25 downto 21)));
                        rsrc2<=to_integer(unsigned(douta(20 downto 16)));
                        rdest<=to_integer(unsigned(douta(15 downto 11)));
                        shamt <= to_integer(unsigned(douta(10 downto 6)));
                        funct <= to_integer(unsigned(douta(5 downto 0)));
                        rtype <= 1;
                        itype<= 0;
                        
                        if(to_integer(unsigned(douta(10 downto 6))) /= 0) then --shamt is not zero, shift operation
--                            counter <= counter+1;
                            state<= shiftoper;
                        else
                        state <= change;
                        end if;
                        
                elsif(douta(31 downto 26)="100011" or douta(31 downto 26)="101011") then --I type
                count <= count+1; 
               -- counter <= counter+1;
                    itype <= 1;
                    rtype <= 0;
                    ifirst<= to_integer(unsigned(douta(31 downto 26)));
                    irs <= to_integer(unsigned(douta(25 downto 21)));
                    ird <= to_integer(unsigned(douta(20 downto 16)));
                    iaddress1 <= to_integer(unsigned(douta(15 downto 0)));--/4;
                    state <= updateir;
--                    state <= change;

-----------------------------------------------------------------------------------------------------------------------------------------

               elsif( douta(31 downto 26)="000101" or douta(31 downto 26) = "000100" ) then    ---beq and bne
                 count <= count+1;
               ifirst<= to_integer(unsigned(douta(31 downto 26)));
               regfirst<=to_integer(unsigned(douta(25 downto 21)));
               regsecond <=to_integer(unsigned(douta(20 downto 16)));
               iaddress1 <= to_integer(unsigned(douta(15 downto 0)));
               state <= comparison;
               
               elsif (douta(31 downto 26) = "000110" or douta(31 downto 26) = "000111" ) then      --blez and bgtz
                count <= count+1;
              ifirst<= to_integer(unsigned(douta(31 downto 26)));
              regfirst<=to_integer(unsigned(douta(25 downto 21)));
              iaddress1 <= to_integer(unsigned(douta(20 downto 0)));
               state <= comparisonzero;
               
               
               elsif (douta(31 downto 26) = "000010") then     ----jump 
                  count <= count+1;
                ifirst<= to_integer(unsigned(douta(31 downto 26)));
                 iaddress1 <= to_integer(unsigned(douta(25 downto 0)));
                 state <= jump ;
                 
                 
                 
                elsif (douta(31 downto 26) = "000000" and douta(5 downto 0) = "001000") then      -- jr 
                  count <= count+1;
                  regindex <= to_integer(unsigned(douta(25 downto 21)));
                  state <= jumpregister;              
                                
                elsif (douta(31 downto 26) = "000011" ) then    ---jal
                  count <= count+1;
                  iaddress1 <= to_integer(unsigned(douta(25 downto 0)));               
                   state <= jumplink;             
----------------------------------------------------------------------------------------------------------------------------------------------
                else 
                wea <="0";
                   state <= idle;
                   
                end if;  
                
                when updateir =>
                
                instruction <= douta;

                        if(ifirst = 35) then
                            irs1 <= (to_integer(signed(reg1(irs)))+iaddress1);
                        elsif (ifirst = 43) then
                            irs1 <= to_integer(signed(reg1(irs))) + iaddress1;
                        end if;
                            state<= change;
--         end if;
                            
                when shiftoper =>
                        if(funct = 0 and rsrc1 = 0) then --shift left
                            state <= shiftlefthelper;
                        elsif( funct = 2) then
                            state <= shiftrighthelper;
                        else state <= idle;
                        end if;
                        
                when shiftlefthelper =>
                               reg1(rdest) <= reg1(rsrc2)(30 downto 0) & "0";
                               rsrc2 <= rdest;
                               if(shamt>1) then
                                    shamt <= shamt-1;
                                    state <= shiftlefthelper;
                                else
                                    sevensegment1 <= reg1(rdest)(15 DOWNTO 0);
                                    state <= idle;
                                end if;

                when shiftrighthelper =>
               
                
                               reg1(rdest) <= "0" & reg1(rsrc2)(31 downto 1);
                                rsrc2<=rdest;                              
                               if(shamt>1) then
                                    shamt <= shamt-1;
                                    state <= shiftrighthelper;
                                else
                                    sevensegment1 <= reg1(rdest)(15 DOWNTO 0);
                                    state <= idle;
                                end if;
                
                when change =>
                
                            instruction <= douta;
                            
                if (douta = "00000000000000000000000000000000") then
                               state <= endstate;
                
                else
                        counter <= counter+1;
                        if(rtype = 1) then
                            manoj<=2;
                            if(douta(5 downto 0)="100000") then --add
                                
                                test<=rdest;
                    --test <= to_integer(unsigned(reg1(rsrc1))+unsigned(reg1(rsrc2)));
                            reg1(rdest)<=std_logic_vector(signed(reg1(rsrc1))+signed(reg1(rsrc2)));
                            sevensegment1 <= reg1(rdest)(15 DOWNTO 0);
                            state <= idle;    
                                elsif(douta(5 downto 0)="100010") then --sub
                                manoj<=2;
                                reg1(rdest)<=std_logic_vector(signed(reg1(rsrc1)) - signed(reg1(rsrc2)));
                                sevensegment1 <= reg1(rdest)(15 DOWNTO 0);
                        state <= idle;
                        end if;
                      elsif (itype = 1) then    --I type
                        if(ifirst = 35) then --lw
                            wea<="0";
                            addra <= std_logic_vector(to_signed(irs1,12));
                            state<= s1;
                            
                        elsif (ifirst = 43) then --sw
                            manoj<=5;
                            wea <= "1";
                            addra <= std_logic_vector(to_signed(irs1,12));
       
                                                    dina <= reg1(ird);
                                                    sevensegment1 <= reg1(ird)(15 DOWNTO 0);
                            state <= idle;
                            end if;
                    else
                    state <= idle;
                    end if;
                end if;     
                    
               when s1 =>  --lw
                        reg1(ird) <= douta;--mem1(irs1);
                        sevensegment1 <= douta(15 DOWNTO 0);
                         state <=idle;
                         
--               when s2 =>
--                        state <= idle;
                        
               when endstate =>
               en1 <= '1';
               if(en2 = '0' and push2 = '0') then
                 sevensegment2 <= std_logic_vector(to_signed(count, 16));
--                 state <= exit1;
                 elsif(en2 = '1' and push2 = '0') then
                 sevensegment2 <= sevensegment1;
--                 state <= endstate;
               end if;
               
              when comparison =>
              
                if (ifirst = 4) then   --beq
             
                     if(reg1(regfirst) = reg1(regsecond)) then
                     count <=  iaddress1 ;
                     sevensegment1 <= std_logic_vector(to_signed(iaddress1, 16));
                      state <=  idle;
                     else 
                      sevensegment1 <= std_logic_vector(to_signed(count, 16));
                      state <= idle;
                      end if;
                
                else                      --(ifirst = 5)   bne
                    
                    if(reg1(regfirst) = reg1(regsecond)) then
                    sevensegment1 <= std_logic_vector(to_signed(count, 16));
                    state <= idle;
                    else 
                     count <=  iaddress1 ;
                      sevensegment1 <= std_logic_vector(to_signed(iaddress1, 16));
                     state <=  idle;
                    end if;
                    
                end if; 
                
                
              when comparisonzero =>
               
               if (ifirst = 6) then           --blez
                   
                   if  ( to_integer(signed(reg1(regfirst)))  <= 0  ) then
                   count <= iaddress1 ;
                    sevensegment1 <= std_logic_vector(to_signed(iaddress1, 16));
                   state <= idle;
                   else
                    sevensegment1 <= std_logic_vector(to_signed(count, 16));
                   state <= idle;
                   end if;
               
               else                             --(ifirst = 7)    bgtz
               
                    if  ( to_integer(signed(reg1(regfirst)))  <= 0  ) then
                     sevensegment1 <= std_logic_vector(to_signed(count, 16));
                     state <= idle;
                    else
                     count <= iaddress1 ;
                                         sevensegment1 <= std_logic_vector(to_signed(iaddress1, 16));
                     state <= idle;
                     end if;
                     
               end if;
               
               
               when jump =>
               
                  count <= iaddress1 ;
                  sevensegment1 <= std_logic_vector(to_signed(iaddress1, 16));
                  state <= idle;
               
               when jumpregister =>
                 
                  count <= to_integer(signed(reg1(regindex)));
                  sevensegment1 <= std_logic_vector(to_signed(to_integer(signed(reg1(regindex))), 16));
                  state <= idle;
                  
               when jumplink =>
                
                 reg1(31) <= std_logic_vector(to_signed(count, 32));
                 count <= iaddress1;
                 sevensegment1 <= std_logic_vector(to_signed(iaddress1, 16));
                 state <= idle;
                 
           
end case;
end if;
--end if;
end process;
end Behavioral;

