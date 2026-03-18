# 核心層 (Core Layer)
負責紀錄遊戲的邏輯、牌局狀態等

在本專案中：
- 實現於 `GameService` 類別中
- 處理遊戲核心邏輯，如：
  - 維護遊戲狀態（當前回合、分數等）
  - 定義卡片資訊結構（`CardInfo`）
  - 生成隨機卡片（`randomGenerateCardInfos`）
  - 檢查卡片是否為正確答案（`checkIsCardAnswer`）
- 不直接與 UI 或網絡層交互，保持邏輯的獨立性

# 應用層 (Application Layer)
負責處理業務邏輯和流程
負責接收後端指令，通知ui更新
點選卡片之後該幹嘛之類的流程判斷放在這

在本專案中：
- 主要實現於 `GameScreen` 類別中
- 作為核心層和 UI 層之間的橋樑
- 處理用戶操作，如：
  - 響應卡片點擊事件（`onCodeCardClicked`）
  - 處理回合切換邏輯（`onTurnNextRound`）
  - 更新遊戲資訊顯示（`onUpdateGameInfo`）
- 調用核心層的方法來執行遊戲邏輯
- 發送信號給 UI 層進行更新

# 基礎設施層 (Infrastructure Layer)
負責與外部系統交互(伺服器)
負責處理與後端的溝通
Socket與後端的溝通實作在這

在本專案中：
- 目前尚未完全實現，但預計會包含：
  - 網絡通信類（如 `NetworkManager`）
  - 處理與伺服器的 WebSocket 連接
  - 發送遊戲操作到伺服器（如 `postCardSelectionToServer`）
  - 接收伺服器指令並轉發給應用層

# 介面層 (UI Layer)
負責處理ui的顯示功能

在本專案中：
- 主要實現於 `GameScreen`、`CodeCard` 等 UI 相關類別中
- 處理所有與用戶界面相關的邏輯，如：
  - 顯示遊戲板和卡片（`setupCodeCards`）
  - 更新回合標籤顏色（`setRoundLabel`）
  - 顯示卡片答案（`showAnswer` in `CodeCard`）
- 接收用戶輸入並轉發給應用層
- 根據應用層的指示更新界面顯示

# 層級關係圖

 
UI -> 應用(核心)-> infra -> 後端









