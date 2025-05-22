# IoT ลูกเต๋าสุ่ม (Final Project CS423 Dice Roll)

โปรเจกต์นี้มีเป้าหมายเพื่อสร้างระบบสุ่มลูกเต๋าโดยใช้ ESP32 ร่วมกับปุ่มกด และแสดงผลผ่านเว็บไซต์แบบเรียลไทม์ พร้อมฟีเจอร์แสดงประวัติและสถิติการทอยลูกเต๋า.

## 🌟 ภาพรวมโปรเจกต์

พัฒนาระบบสุ่มลูกเต๋าโดยใช้ ESP32 และปุ่มกด โดยเมื่อผู้ใช้กดปุ่ม, ESP32 จะทำการสุ่มตัวเลข 1-6 แล้วส่งผลลัพธ์ผ่าน Wi-Fi ไปยัง Node.js Server. เซิร์ฟเวอร์จะบันทึกข้อมูลลงในไฟล์ JSON และ Frontend (หน้าเว็บ) จะดึงข้อมูลนี้ไปแสดงผลเป็นรูป SVG ลูกเต๋า, ตารางประวัติการทอย, และสถิติต่างๆ แบบเรียลไทม์.

**วัตถุประสงค์:**

* พัฒนาระบบสุ่มลูกเต๋าโดยใช้ ESP32 และปุ่มกด.
* เชื่อมต่อ ESP32 กับ Wi-Fi เพื่อส่งผลลัพธ์ไปยังเซิร์ฟเวอร์.
* พัฒนา Backend API ด้วย Node.js สำหรับรับ, ประมวลผล, และจัดเก็บข้อมูลทอยลูกเต๋าลงใน JSON.
* พัฒนา Frontend แสดงผล SVG, ประวัติ, และสถิติ.
* ศึกษาและทำความเข้าใจกระบวนการทำงานของระบบ IoT แบบครบวงจร.

---

## 🛠️ ส่วนประกอบของระบบ

### ฮาร์ดแวร์

* ESP32 Dev Module 
* ปุ่มกด (VCC, GND, OUT) 
* สาย Jumper Wires 
* USB Power 

### ซอฟต์แวร์และเครื่องมือ

* Arduino IDE 
* VS Code 
* Node.js 
* Live Server (VS Code Extension) 
* Git 
* Web Browser (เช่น Chrome) 

---

## 🏗️ System Architecture

1.  **ผู้ใช้กดปุ่ม:** ESP32 ตรวจจับการกดปุ่มและสุ่มตัวเลข 1-6.
2.  **ESP32 ส่งข้อมูล:** ส่งผลการสุ่มผ่าน HTTP POST request ไปยัง Node.js Server ผ่าน Wi-Fi.
3.  **Node.js Server ประมวลผล:** รับข้อมูล, ประมวลผล, และจัดเก็บข้อมูลการทอยลูกเต๋าลงในไฟล์ JSON.
4.  **Frontend แสดงผล:** Web Browser ใช้ Fetch API ดึงข้อมูลประวัติและสถิติจาก Server เพื่อแสดงผลเป็นรูป SVG ลูกเต๋า, ตารางประวัติ, และสถิติ โดยมีการอัปเดตอัตโนมัติ.

ESP32 จะเชื่อมต่อกับโมดูลปุ่มกดเพื่อตรวจจับการกดปุ่มและสุ่มเลขลูกเต๋า.

| ปุ่มกด | ESP32   |
| :----- | :------ |
| VCC    | 3V3     |
| GND    | GND     |
| OUT    | GPIO4   |


---

### Clone Repository

เปิด VS Code และเปิด Terminal จากนั้นใช้คำสั่ง:

```bash
git clone https://github.com/ThanaboonChantasawat/CS423-Final-Project-Dice.git
cd CS423-Final-Project-Dice 
```
---
### ตั้งค่า ESP32

ส่วนนี้จะเป็นขั้นตอนการตั้งค่าโปรแกรมบนตัวบอร์ด ESP32 ของเราครับ

1.  เปิดไฟล์ `dice/dice.ino` ด้วย Arduino IDE

2.  **ติดตั้ง Board Manager สำหรับ ESP32:**
    * ไปที่ `Boards Manager`
    * ค้นหาคำว่า "esp32" (by Espressif Systems) แล้วกด Install
    * ทำการเลือกบอร์ดให้เรียบร้อย

4.  **ในไฟล์ `dice.ino` แก้ไขข้อมูล Wi-Fi:**
    ```cpp
    const char* ssid = "ชื่อWIFIของคุณ";       // ใส่ชื่อ Wi-Fi ของคุณตรงนี้
    const char* password = "รหัสผ่านWIFIของคุณ"; // ใส่รหัสผ่าน Wi-Fi ของคุณตรงนี้
    ```
    * *หมายเหตุ: ESP32 ส่วนใหญ่มักมีปัญหากับ Wi-Fi 5GHz, **กรุณาใช้ 2.4GHz** นะครับ*

5.  **แก้ไข IP Address ของ Server:**
    ```cpp
    String serverName = "http://[Your_IP]:3000/api/dice_roll"; // เปลี่ยน [Your_IP] เป็น IP Address ของเครื่องคอมพิวเตอร์ที่รัน Node.js Server
    ```
    * *คุณสามารถหา IP Address ของเครื่องคอมพิวเตอร์ (ที่รัน Node.js Server) ได้โดยใช้คำสั่ง `ipconfig` (สำหรับ Windows) หรือ `ifconfig` (สำหรับ macOS/Linux) ใน Terminal หรือ Command Prompt*

6.  กดปุ่ม **Upload**

7.  เมื่ออัปโหลดเสร็จสิ้น เปิด **Serial Monitor** และตั้งค่า Baud rate เป็น **115200** เพื่อดู Log การทำงานและการเชื่อมต่อ Wi-Fi ของ ESP32
### ตั้งค่า Backend Server (Node.js)

1.  **ติดตั้ง Node.js:**
    * ติดตั้ง Node.js
    * สามารถดาวน์โหลดและติดตั้ง Node.js ได้จาก [nodejs.org](https://nodejs.org/)

2.  **ไปยังโฟลเดอร์ `server`:**
      ```
      cd server
      npm install
      ```   
3.  **รันเซิร์ฟเวอร์ Backend:**
   
        node app.js

    
    * เมื่อเซิร์ฟเวอร์เริ่มทำงานสำเร็จ น่าจะแสดง:
        ```
        Server is running on http://localhost:3000
        Storing history in: ...\CS423-Final-Project-Dice\server\dice_history.json
        ```

ส่วนนี้จะอธิบายวิธีการเปิดหน้าเว็บ (Frontend) ของคุณเพื่อดูผลลัพธ์และโต้ตอบกับระบบ

1.  **เปลี่ยน IP ใน index.html**
    ```
    const API_HISTORY_URL = 'http://[ํYour_IP]:3000/api/history'; เปลี่ยน [ํYour_IP] เป็น ip เครื่อง
    ```

1.  **ติดตั้ง Extension "Live Server" ใน VS Code:**
    * Live Server เป็น Extension ของ VS Code ที่รันหน้าเว็บ HTML/CSS/JS บน local development server และมีฟีเจอร์ auto-reload เมื่อมีการแก้ไขโค้ด
    * ไปที่แท็บ Extensions ใน VS Code
    * ค้นหา "Live Server" (โดย Ritwick Dey) แล้วกด Install

2.  **เปิดหน้าเว็บด้วย Live Server:**
    * ใน VS Code Explorer, คลิกขวาที่ไฟล์ `index.html`
    * เลือก "Open with Live Server" จาก context menu
    * Live Server จะเริ่มทำงานและเปิดหน้าเว็บขึ้นมาในเว็บเบราว์เซอร์เริ่มต้นโดยอัตโนมัติ
  
ทำการกดปุ่ม จะทำการแสดงผลลัพธ์ในการสุ่มหน้า 1-6

![image](https://github.com/user-attachments/assets/a81169ab-e089-48e5-a92b-ab0e85353bda)
