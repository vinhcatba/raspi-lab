#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>
#include <string.h>

// Các chân GPIO nối với LCD
#define LCD_RS  0       // Register select pin
#define LCD_E   5       // Enable Pin
#define LCD_D4  6       // Data pin D4
#define LCD_D5  13      // Data pin D5
#define LCD_D6  19      // Data pin D6
#define LCD_D7  26      // Data pin D7
// LCD -------- Ras
// VCC          5V
// VSS(GND)     GND 
// RW           GND     // Nối chân RW xuống LOW (GND) để dùng lcd write-only (lcd 5V nên không thể để ras read)


#define LCD_WIDTH 16    // Độ dài 1 dòng
#define LCD_CHR 1       // Chế độ ghi ký tự 
#define LCD_CMD 0       // Chế độ ghi lệnh

#define False 0
#define True 1
 
#define LCD_LINE_1 0x80 // Địa chỉ dòng 1 trên LCD RAM
#define LCD_LINE_2 0xC0 // Địa chỉ dòng 2 trên LCD RAM

/* Các nguyên mẫu hàm */
void lcd_init();                            // Initialize lcd
void lcd_write(int bits, int mode);         // Ghi dữ liệu tới lcd
void lcd_toggle_enable();                   // Bật tắt enable pin (để hiển thị ký tự)
void lcd_string(char *str);                 // Hiểu thị 1 chuỗi lên lcd
void gpiodout(int lineOffset, int value);   // Wrapper để ghi ra GPIO bằng libgpiod

struct gpiod_chip *chip;

int main(int argc, char *argv[]){
    const char *chipname = "gpiochip0";
    chip = gpiod_chip_open_by_name(chipname);   // Mở chip gpiochip0

    lcd_init(); 
    if(argc > 1)
        lcd_string(argv[1]);
    else 
        lcd_string("Hello world");
    sleep(3);
    lcd_write(0x01, LCD_CMD);   // Clear

    // Ghi 0 cho tất cả GPIO đã mở
    int lines[] = {0, 5, 6, 13, 19, 26};
    for (int i=0 ; i<(sizeof(lines)/sizeof(int)); i++){
        gpiodout(lines[i], 0);
    }

    // Đóng chip
    gpiod_chip_close(chip);
    return 0;
}

/* Ghi các lệnh cơ bản để khởi tạo LCD */
void lcd_init(){
    lcd_write(0x28,LCD_CMD); // Chọn chế độ ghi 4bits 2 hàng
    lcd_write(0x0C,LCD_CMD); // Display On, Cursor Off, Blink Off
    lcd_write(0x01,LCD_CMD); // Clear
}

/* Ghi 1 byte tới lcd, chế độ 4 bit */ 
void lcd_write(int bits, int mode){
    // bits = data
    // mode = True  for character
    //        False for command
    
    gpiodout(LCD_RS, mode); // RS=0 để ghi lệnh, RS=1 để ghi ký tự ASCII
    
    // High bits (4 bit bên trái) (xxxx ----)
    gpiodout(LCD_D4, False);
    gpiodout(LCD_D5, False);
    gpiodout(LCD_D6, False);
    gpiodout(LCD_D7, False);
    if ((bits&0x10)==0x10)      // bit thứ 1 từ phải sang (xxx1 ----)
        gpiodout(LCD_D4, True);
    if ((bits&0x20)==0x20)      // bit thứ 2 từ phải sang (xx1x ----)
        gpiodout(LCD_D5, True);
    if ((bits&0x40)==0x40)      // bit thứ 3 từ phải sang (x1xx ----)
        gpiodout(LCD_D6, True);
    if ((bits&0x80)==0x80)      // bit thứ 4 từ phải sang (1xxx ----)
        gpiodout(LCD_D7, True);

    lcd_toggle_enable();        // bật 'Enable' pin
    
    // Low bits (4 bits bên phải) (---- xxxx)
    gpiodout(LCD_D4, False);
    gpiodout(LCD_D5, False);
    gpiodout(LCD_D6, False);
    gpiodout(LCD_D7, False);
    if ((bits&0x01)==0x01)      // bit thứ 1 từ phải sang (---- xxx1)
        gpiodout(LCD_D4, True);
    if ((bits&0x02)==0x02)      // bit thứ 2 từ phải sang (---- xx1x)
        gpiodout(LCD_D5, True);
    if ((bits&0x04)==0x04)      // bit thứ 3 từ phải sang (---- x1xx)
        gpiodout(LCD_D6, True);
    if ((bits&0x08)==0x08)      // bit thứ 4 từ phải sang (---- 1xxx)
        gpiodout(LCD_D7, True);
    
    lcd_toggle_enable();        // bật 'Enable' pin
}

/* Bật 'Enable' pin để bắt đầu ghi */ 
void lcd_toggle_enable(){
    usleep(500);
    gpiodout(LCD_E, True);
    usleep(500);
    gpiodout(LCD_E, False);
    usleep(500);
}

/* wrapper để ghi GPIO bằng GPIO Broadcom (gpio offset) */
void gpiodout(int lineOffset, int value){
    struct gpiod_line *line;                        
    line = gpiod_chip_get_line(chip, lineOffset);   // get line từ số chân GPIO Broadcom (gpio offset)
    gpiod_line_request_output(line, "LCD", 0);      // request line là output, giá trị khởi đầu là 0
    gpiod_line_set_value(line, value);              // đặt giá trị line là value
    gpiod_line_release(line);                       // release line
}


/* Ghi chuỗi ra lcd */
void lcd_string(char *str){
    int length = strlen(str);
    if (length > LCD_WIDTH){            // Nếu độ dài chuỗi lớn hơn 1 dòng thì in nó thành 2 dòng
        // Ghi dòng 1
        lcd_write(LCD_LINE_1, LCD_CMD);
        for(int i=0; i<LCD_WIDTH; i++){
            lcd_write(str[i], LCD_CHR);
        }
        // Ghi dòng 2
        if(length > 2*LCD_WIDTH){    // Nếu độ dài chuỗi lớn hơn 2 dòng thì in "..." ở cuối dòng 2
            lcd_write(LCD_LINE_2, LCD_CMD);
            for(int i=LCD_WIDTH; i<2*LCD_WIDTH-3; i++){
                lcd_write(str[i], LCD_CHR);
            }
            for(int i=0; i<3; i++){
                lcd_write('.', LCD_CHR);
            }
        }else{
            lcd_write(LCD_LINE_2, LCD_CMD);
            for(int i=LCD_WIDTH; i<length; i++){
                lcd_write(str[+i], LCD_CHR);
            }
        }
    }else{                              // Ngược lại thì ghi vào dòng 1
        lcd_write(LCD_LINE_1, LCD_CMD);
        for(int i=0; i<length; i++){
            lcd_write(str[i], LCD_CHR);
        }
    }
}