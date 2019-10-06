使用程式前需先使用指令make生成 405235035_hw4

輸入./405235035_hw4 即可執行程式 
此程式執行時會自動讀取當前所在位置下的input.txt檔案
執行後會輸出一個檔案，名為output.txt
內容為建立出的每個edge最早可執行的時間和最晚須執行的時間和兩者的差異和判斷是否為critical path

此程式先一行一行將每個edge的內容讀取出來(起點、終點、權重)
再利用linked lists的方式將每條edge的資料插入到兩個不同的adjacency list中
再利用adjacency list建構出每個vertex的ee(Early Event Time)、le(Late Event Time)的table
最後利用前面所得的的資訊算出每個edge的early、late、diff和是否為critical

最多的 VERTEXE 的數量可用 MAX_VERTEX_SIZE 修改，預設為64
最多的 EDGE 數量可用 MAX_EDGE_SIZE 修改，預設為256


額外功能：
1.此程式可判斷graph是否有迴圈
2.此程式結束時會將adjacency list中所malloc出的節點全數釋放，不會造成memory leakage

405235035 資工三 王博輝 b121417393@gmail.com
