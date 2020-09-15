----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 18.02.2020 18:00:57
-- Design Name: 
-- Module Name: sevensegment - Behavioral
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

entity sevensegment is
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
end sevensegment;

architecture Behavioral of sevensegment is

TYPE state_type is (s0, s1, s2, s3);

signal state : state_type := s0;
signal temp : integer := 0;
signal clk1 : std_logic:='0';
signal arr : std_logic_vector(2 downto 0);
signal default :std_logic_vector(6 downto 0) := "1000000";

signal A : std_logic_vector(6 downto 0);
signal B : std_logic_vector(6 downto 0);
signal C : std_logic_vector(6 downto 0);
signal D : std_logic_vector(6 downto 0);


begin

process(clk)
    begin
        if(clk'event and clk = '1') then
        temp <= temp+1;
        if(temp = 5208) then
            clk1 <= not clk1;
            temp <= 0;
        end if;
      end if;
    end process;                                             

process(clk1, state, en1)
    begin
    
    if(clk1'event and clk1 = '1') then
    
    if(en1 = '0') then
        an0 <= '1';
        an1 <= '1';
        an2 <= '1';
        an3 <= '1';
     else
     
if (to_integer(unsigned(bitset1)) = 0) then A<="1000000";
        elsif (to_integer(unsigned(bitset1))=1) then  A <= "1111001";
        elsif (to_integer(unsigned(bitset1)) = 2) then  A<="0100100";
      elsif  (to_integer(unsigned(bitset1)) = 3) then   A<="0110000";
       elsif  (to_integer(unsigned(bitset1)) = 4 ) then  A<="0011001";
        elsif  (to_integer(unsigned(bitset1)) = 5) then  A<="0010010";
        elsif (to_integer(unsigned(bitset1)) = 6) then  A<="0000010";
        elsif (to_integer(unsigned(bitset1)) = 7) then  A<="1111000";
        elsif (to_integer(unsigned(bitset1)) = 8) then   A<="0000000";
        elsif (to_integer(unsigned(bitset1)) = 9) then  A<="0010000";
        elsif (to_integer(unsigned(bitset1)) = 10) then  A<="0001000";
        elsif (to_integer(unsigned(bitset1)) = 11) then  A<="0000011";
        elsif (to_integer(unsigned(bitset1)) = 12) then  A<="1000110";
        elsif (to_integer(unsigned(bitset1)) = 13) then  A<="0100001";
        elsif (to_integer(unsigned(bitset1)) = 14) then  A<="0000110";
        elsif (to_integer(unsigned(bitset1)) = 15) then  A<= "0001110"; 
 end if;       

if (to_integer(unsigned(bitset2)) = 0) then B<="1000000";
        elsif (to_integer(unsigned(bitset2))=1) then  B <= "1111001";
        elsif (to_integer(unsigned(bitset2)) = 2) then  B<="0100100";
      elsif  (to_integer(unsigned(bitset2)) = 3) then   B<="0110000";
       elsif  (to_integer(unsigned(bitset2)) = 4 ) then  B<="0011001";
        elsif  (to_integer(unsigned(bitset2)) = 5) then  B<="0010010";
        elsif (to_integer(unsigned(bitset2)) = 6) then  B<="0000010";
        elsif (to_integer(unsigned(bitset2)) = 7) then  B<="1111000";
        elsif (to_integer(unsigned(bitset2)) = 8) then   B<="0000000";
        elsif (to_integer(unsigned(bitset2)) = 9) then  B<="0010000";
        elsif (to_integer(unsigned(bitset2)) = 10) then  B<="0001000";
        elsif (to_integer(unsigned(bitset2)) = 11) then  B<="0000011";
        elsif (to_integer(unsigned(bitset2)) = 12) then  B<="1000110";
        elsif (to_integer(unsigned(bitset2)) = 13) then  B<="0100001";
        elsif (to_integer(unsigned(bitset2)) = 14) then  B<="0000110";
        elsif (to_integer(unsigned(bitset2)) = 15) then  B<= "0001110"; 
 end if; 
 
 if (to_integer(unsigned(bitset3)) = 0) then C<="1000000";
         elsif (to_integer(unsigned(bitset3))=1) then  C <= "1111001";
         elsif (to_integer(unsigned(bitset3)) = 2) then  C<="0100100";
       elsif  (to_integer(unsigned(bitset3)) = 3) then   C<="0110000";
        elsif  (to_integer(unsigned(bitset3)) = 4 ) then  C<="0011001";
         elsif  (to_integer(unsigned(bitset3)) = 5) then  C<="0010010";
         elsif (to_integer(unsigned(bitset3)) = 6) then  C<="0000010";
         elsif (to_integer(unsigned(bitset3)) = 7) then  C<="1111000";
         elsif (to_integer(unsigned(bitset3)) = 8) then   C<="0000000";
         elsif (to_integer(unsigned(bitset3)) = 9) then  C<="0010000";
         elsif (to_integer(unsigned(bitset3)) = 10) then  C<="0001000";
         elsif (to_integer(unsigned(bitset3)) = 11) then  C<="0000011";
         elsif (to_integer(unsigned(bitset3)) = 12) then  C<="1000110";
         elsif (to_integer(unsigned(bitset3)) = 13) then  C<="0100001";
         elsif (to_integer(unsigned(bitset3)) = 14) then  C<="0000110";
         elsif (to_integer(unsigned(bitset3)) = 15) then  C<= "0001110"; 
  end if; 
  
  if (to_integer(unsigned(bitset4)) = 0) then D<="1000000";
          elsif (to_integer(unsigned(bitset4))=1) then   D<= "1111001";
          elsif (to_integer(unsigned(bitset4)) = 2) then  D<="0100100";
        elsif  (to_integer(unsigned(bitset4)) = 3) then   D<="0110000";
         elsif  (to_integer(unsigned(bitset4)) = 4 ) then  D<="0011001";
          elsif  (to_integer(unsigned(bitset4)) = 5) then  D<="0010010";
          elsif (to_integer(unsigned(bitset4)) = 6) then  D<="0000010";
          elsif (to_integer(unsigned(bitset4)) = 7) then  D<="1111000";
          elsif (to_integer(unsigned(bitset4)) = 8) then   D<="0000000";
          elsif (to_integer(unsigned(bitset4)) = 9) then  D<="0010000";
          elsif (to_integer(unsigned(bitset4)) = 10) then  D<="0001000";
          elsif (to_integer(unsigned(bitset4)) = 11) then  D<="0000011";
          elsif (to_integer(unsigned(bitset4)) = 12) then  D<="1000110";
          elsif (to_integer(unsigned(bitset4)) = 13) then  D<="0100001";
          elsif (to_integer(unsigned(bitset4)) = 14) then  D<="0000110";
          elsif (to_integer(unsigned(bitset4)) = 15) then  D<= "0001110"; 
   end if;     
    end if;
    
        case state is
            when s0 =>
                an0 <= '0';
                an1 <= '1';
                an2 <= '1';
                an3 <= '1';
                seg <= A;
                state<= s1;
           when s1 => 
                an0 <= '1';
                an1 <= '0';
                an2 <= '1';
                an3 <= '1';
                seg <= B;
                state<= s2;
           when s2 => 
                an0 <= '1';
                an1 <= '1';
                an2 <= '0';
                an3 <= '1';
                seg <= C;
                state<= s3;
        when s3 => 
                an0 <= '1';
                an1 <= '1';
                an2 <= '1';
                an3 <= '0';
                seg <= D;
                state<= s0;     
            end case;
            end if;
 end process;
end Behavioral;


