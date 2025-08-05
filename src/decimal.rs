#[derive(Clone, Copy, Debug)]
pub struct Dec {
    pub bits: [u32; 4],
}

impl Dec {
    pub const BEGIN: usize = 2;
    pub const METAINFO: usize = 3;
    pub const END: usize = 0;
    pub const MAX_POS: usize = 95;

    pub fn new() -> Self {
        Self { bits: [0; 4] }
    }

    pub fn is_zero(&self) -> bool {
        self.bits[0] == 0 && self.bits[1] == 0 && self.bits[Self::BEGIN] == 0
    }

    pub fn get_sign(&self) -> bool {
        ((1_u32 << 31) & self.bits[Self::METAINFO]) != 0
    }

    pub fn get_scale(&self) -> u32 {
        (self.bits[Self::METAINFO] >> 16) & 0xFF
    }

    // Получаем бит по позиции (0..=95)
    pub fn get_bit(&self, pos: u32) -> bool {
        let pos = pos.min(95); // Ограничиваем 95

        let part = Self::BEGIN - (pos / 32) as usize; // Определяем, в каком bits[] находится бит
        let local_pos = pos % 32;

        (self.bits[part] >> local_pos) & 1 != 0
    }

    // Установить знак (0 или 1)
    pub fn set_sign(&mut self, sign_value: u32) -> bool {
        if sign_value > 1 {
            return true; // ошибка
        }
        self.bits[Self::METAINFO] = (self.bits[Self::METAINFO] & !0x80000000) | (sign_value << 31);
        false
    }

    // Установить масштаб (от 0 до 28)
    pub fn set_scale(&mut self, scale_value: u32) -> bool {
        if scale_value > 28 {
            return true; // ошибка
        }
        self.bits[Self::METAINFO] = (self.bits[Self::METAINFO] & !0x00FF0000) | (scale_value << 16);
        false
    }

    // Установить бит по позиции (0..=95)
    pub fn set_bit(&mut self, pos: u32, val: bool) -> bool {
        if pos > 95 {
            return true; // ошибка
        }

        let part = Self::BEGIN - (pos / 32) as usize;
        let local_pos = pos % 32;

        if val {
            self.bits[part] |= 1 << local_pos;
        } else {
            self.bits[part] &= !(1 << local_pos);
        }

        false
    }
}

#[derive(Clone, Copy, Debug)]
pub struct BigDec {
    pub bits: [u32; 8],
}

impl BigDec {
    pub const BEGIN: usize = 6;
    pub const METAINFO: usize = 7;
    pub const END: usize = 0;
    pub const MAX_POS: usize = 223;

    pub fn new() -> Self {
        Self { bits: [0; 8] }
    }

    // Создаём BigDec из 8 u32
    pub fn from_bits(b0: u32, b1: u32, b2: u32, b3: u32,
                     b4: u32, b5: u32, b6: u32, b7: u32) -> Self {
        BigDec {
            bits: [b0, b1, b2, b3, b4, b5, b6, b7],
        }
    }

    // Проверяем, является ли большое число нулём
    pub fn is_zero(&self) -> bool {
        self.bits[0] == 0 &&
        self.bits[1] == 0 &&
        self.bits[2] == 0 &&
        self.bits[3] == 0 &&
        self.bits[4] == 0 &&
        self.bits[5] == 0 &&
        self.bits[Self::BEGIN] == 0
    }

    // Получаем знак числа (true — отрицательное)
    pub fn get_sign(&self) -> bool {
        ((1_u32 << 31) & self.bits[Self::METAINFO]) != 0
    }

    // Получаем масштаб (scale)
    pub fn get_scale(&self) -> u32 {
        (self.bits[Self::METAINFO] >> 16) & 0xFF
    }

    // Получаем бит по позиции (0..=223)
    pub fn get_bit(&self, pos: u32) -> bool {
        let pos = pos.min(223); // Ограничиваем 223

        let part = Self::BEGIN - (pos / 32) as usize; // Определяем, в каком bits[] находится бит
        let local_pos = pos % 32;

        (self.bits[part] >> local_pos) & 1 != 0
    }

    // Установить знак (0 или 1)
    pub fn set_sign(&mut self, sign_value: u32) -> bool {
        if sign_value > 1 {
            return true; // ошибка
        }
        self.bits[Self::METAINFO] = (self.bits[Self::METAINFO] & !0x80000000) | (sign_value << 31);
        false
    }

    // Установить масштаб (от 0 до 28)
    pub fn set_scale(&mut self, scale_value: u32) -> bool {
        if scale_value > 28 {
            return true; // ошибка
        }
        self.bits[Self::METAINFO] = (self.bits[Self::METAINFO] & !0x00FF0000) | (scale_value << 16);
        false
    }

    // Установить бит по позиции (0..=223)
    pub fn set_bit(&mut self, pos: u32, val: bool) -> bool {
        if pos > 223 {
            return true; // ошибка
        }

        let part = Self::BEGIN - (pos / 32) as usize;
        let local_pos = pos % 32;

        if val {
            self.bits[part] |= 1 << local_pos;
        } else {
            self.bits[part] &= !(1 << local_pos);
        }

        false
    }

        // Битовый сдвиг влево на shift_value бит (максимум 31)
    pub fn shift_left(mut self, shift_value: u32) -> Self {
        let shift_value = shift_value.min(31);
        let mut memory = 0;

        for i in (0..=Self::BEGIN).rev() {
            let temp = self.bits[i];
            self.bits[i] = (temp << shift_value) | memory;
            memory = temp >> (32 - shift_value);
        }

        self
    }

    // Конвертируем Dec в BigDec
    pub fn from_dec(dec: Dec) -> Self {
        let mut big = Self::new();

        // Копируем знак и scale из METAINFO
        big.bits[Self::METAINFO] = dec.bits[Dec::METAINFO];

        // Копируем мантиссу
        big.bits[Self::BEGIN - 0] = dec.bits[Dec::BEGIN - 0];
        big.bits[Self::BEGIN - 1] = dec.bits[Dec::BEGIN - 1];
        big.bits[Self::BEGIN - 2] = dec.bits[Dec::BEGIN - 2];

        big
    }

    // Умножает число на 10 (in-place)
    pub fn multiply_by_10(&mut self) -> bool {
        let temp1 = self.shift_left(3); // *8
        let temp2 = self.shift_left(1); // *2

        let mut carry: u64 = 0;

        for i in (Dec::BEGIN..=Self::BEGIN).rev() {
            let sum = (temp1.bits[i] as u64) + (temp2.bits[i] as u64) + carry;
            self.bits[i] = sum as u32;
            carry = sum >> 32;
        }

        // Проверяем, не превышает ли scale значение 28
        let new_scale = self.get_scale() + 1;
        if new_scale > 28 {
            return true; // ошибка: масштаб стал слишком большим
        }

        self.set_scale(new_scale);
        false
    }

    // Выравниваем масштабы двух чисел до одинакового значения
    pub fn normalize_scales(big1: &mut Self, big2: &mut Self) -> bool {
        while big1.get_scale() < big2.get_scale() {
            if big1.multiply_by_10() {
                return true; // ошибка при увеличении scale
            }
        }
        while big2.get_scale() < big1.get_scale() {
            if big2.multiply_by_10() {
                return true; // ошибка при увеличении scale
            }
        }
        false
    }
}

#[cfg(test)]
mod big_decimal_tests {
    use super::*;

    fn assert_big_decimal_eq(a: BigDec, b: BigDec) {
        for i in 0..8 {
            assert_eq!(a.bits[i], b.bits[i], "Mismatch at index {}", i);
        }
    }

    #[test]
    fn test_shift_left_one_bit() {
        let input = BigDec::from_bits(0, 0, 0, 0, 0, 0, 1, 0); // число 1
        let expected = BigDec::from_bits(0, 0, 0, 0, 0, 0, 2, 0);

        let result = input.shift_left(1);
        assert_big_decimal_eq(result, expected);
    }

    #[test]
    fn test_shift_left_three_bits() {
        let input = BigDec::from_bits(0, 0, 0, 0, 0, 0, 1, 0); // число 1
        let expected = BigDec::from_bits(0, 0, 0, 0, 0, 0, 8, 0); // *8

        let result = input.shift_left(3);
        assert_big_decimal_eq(result, expected);
    }

    #[test]
    fn test_shift_left_with_carry_between_words() {
        let input = BigDec::from_bits(0, 0, 0, 0, 0, 0, 0xFFFFFFFF, 0);
        let expected = BigDec::from_bits(0, 0, 0, 0, 0, 1, 0xFFFFFFFE, 0);

        let result = input.shift_left(1);
        assert_big_decimal_eq(result, expected);
    }

    #[test]
    fn test_shift_left_does_not_touch_meta() {
        let input = BigDec::from_bits(1, 0, 0, 0, 0, 0, 0, 0xAABBCCDD);
        let result = input.shift_left(1);
        assert_eq!(result.bits[BigDec::METAINFO], 0xAABBCCDD);
    }

    #[test]
    fn test_normalize_equal_scales() {
        let mut num1 = BigDec::from_bits(123, 0, 0, 0, 0, 0, 0, 0);
        let mut num2 = BigDec::from_bits(456, 0, 0, 0, 0, 0, 0, 0);

        num1.set_scale(5);
        num2.set_scale(5);

        let original_num1 = num1;
        let original_num2 = num2;

        BigDec::normalize_scales(&mut num1, &mut num2);

        assert_big_decimal_eq(num1, original_num1);
        assert_big_decimal_eq(num2, original_num2);
        assert_eq!(num1.get_scale(), num2.get_scale());
    }

    #[test]
    fn test_normalize_num1_greater_scale() {
        let mut num1 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 100, 0);
        let mut num2 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 10, 0);

        num1.set_scale(3);
        num2.set_scale(1);

        BigDec::normalize_scales(&mut num1, &mut num2);

        assert_eq!(num2.get_scale(), 3);
        assert_eq!(num2.bits[BigDec::BEGIN], 1000);
        assert_eq!(num1.get_scale(), 3);
    }

    #[test]
    fn test_normalize_num2_greater_scale() {
        let mut num1 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 10, 0);
        let mut num2 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 100, 0);

        num1.set_scale(1);
        num2.set_scale(3);

        BigDec::normalize_scales(&mut num1, &mut num2);

        assert_eq!(num1.get_scale(), 3);
        assert_eq!(num1.bits[BigDec::BEGIN], 1000);
        assert_eq!(num2.get_scale(), 3);
    }

    #[test]
    fn test_multiply_by_10_basic() {
        let mut val = BigDec::from_bits(0, 0, 0, 0, 0, 0, 1, 0);
        val.multiply_by_10();
        val.multiply_by_10();

        let expected = BigDec::from_bits(0, 0, 0, 0, 0, 0, 100, 0x020000);

        assert_big_decimal_eq(val, expected);
    }

    #[test]
    fn test_normalize_scales_increase_first() {
        let mut num1 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 123, 0);
        let mut num2 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 123000, 0);

        num1.set_scale(2);
        num2.set_scale(5);

        let mut expected1 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 123000, 0);
        let mut expected2 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 123000, 0);

        expected1.set_scale(5);
        expected2.set_scale(5);

        BigDec::normalize_scales(&mut num1, &mut num2);

        assert_big_decimal_eq(num1, expected1);
        assert_big_decimal_eq(num2, expected2);
        assert_eq!(num1.get_scale(), 5);
        assert_eq!(num2.get_scale(), 5);
    }

    #[test]
    fn test_normalize_scales_increase_second() {
        let mut num1 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 123000, 0);
        let mut num2 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 123, 0);

        num1.set_scale(5);
        num2.set_scale(2);

        let mut expected1 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 123000, 0);
        let mut expected2 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 123000, 0);

        expected1.set_scale(5);
        expected2.set_scale(5);

        BigDec::normalize_scales(&mut num1, &mut num2);

        assert_big_decimal_eq(num1, expected1);
        assert_big_decimal_eq(num2, expected2);
        assert_eq!(num1.get_scale(), 5);
        assert_eq!(num2.get_scale(), 5);
    }

    #[test]
    fn test_normalize_scales_equal() {
        let mut num1 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 12345, 0);
        let mut num2 = BigDec::from_bits(0, 0, 0, 0, 0, 0, 12345, 0);

        num1.set_scale(3);
        num2.set_scale(3);

        BigDec::normalize_scales(&mut num1, &mut num2);

        assert_big_decimal_eq(num1, num2);
        assert_eq!(num1.get_scale(), 3);
        assert_eq!(num2.get_scale(), 3);
    }
}