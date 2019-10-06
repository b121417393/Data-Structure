使用程式前需先使用指令make生成 405235035_hw3

輸入./405235035_hw3 即可執行程式 
此程式執行時會自動讀取當前所在位置下的text.txt檔案
執行後會輸出一個檔案，名為code.txt，內容為利用text.txt檔案中所有的字元來建造一個Huffman Coding Tree

此程式先利用linked lists將所有字元出現的順序及頻率記錄下來
再利用priority queues的方法用此linked list建造一個min heap tree
再用min heap tree構築出一個Huffman Coding Tree，回過頭來找出每一個字元的編碼方式放回至linked list中
再重讀一次code.txt的內容，搭配linked list的資料將每一個字元做編碼後輸出。

最大priority queues的大小可用 MAX_QUEUE_SIZE 修改，預設為4096
最大字元編碼長度可用 MAX_CODING_SIZE 修改，預設為32
最大加密字串長度可用 MAX_ENCRYPTE_SIZE 修改，預設為10240

額外功能：
1.此程式可接受多行的字串輸入，並會依全部字串的全部字元來進行編碼分析，其加密結果也會分成多行輸出
2.此程式結束時會將因linked list 及 tree所malloc出的節點全數釋放，不會造成memory leakage

405235035 資工三 王博輝 b121417393@gmail.com
