/*
Given a 2D grid consists of 0s (land) and 1s (water).  An island is a maximal 4-directionally
connected group of 0s and a closed island is an island totally (all left, top, right, bottom)
surrounded by 1s.

Return the number of closed islands.

Example 1:

Input: grid = [[1,1,1,1,1,1,1,0],[1,0,0,0,0,1,1,0],[1,0,1,0,1,1,1,0],[1,0,0,0,0,1,0,1],[1,1,1,1,1,1,1,0]]
Output: 2
Explanation:
Islands in gray are closed because they are completely surrounded by water (group of 1s).

Example 2:
Input: grid = [[0,0,1,0,0],[0,1,0,1,0],[0,1,1,1,0]]
Output: 1

Example 3:
Input: grid = [[1,1,1,1,1,1,1],
               [1,0,0,0,0,0,1],
               [1,0,1,1,1,0,1],
               [1,0,1,0,1,0,1],
               [1,0,1,1,1,0,1],
               [1,0,0,0,0,0,1],
               [1,1,1,1,1,1,1]]
Output: 2

Constraints:

    1 <= grid.length, grid[0].length <= 100
    0 <= grid[i][j] <=1

*/

use std::ops::{Add, Mul};

pub trait IsBinary {
    fn is_binary(&self) -> bool;
}

macro_rules! impl_is_binary {
    ($($t:ty), *) => {
        $(
            impl IsBinary for $t {
                fn is_binary(&self) -> bool {
                    *self == 0 || *self == 1
                }
            }
        )*
    };
}

impl_is_binary!(i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize);

pub trait MatrixElement:
    Copy + Default + PartialEq + Add<Output = Self> + Mul<Output = Self>
{
}

impl<T> MatrixElement for T where
    T: Copy + Default + PartialEq + Add<Output = Self> + Mul<Output = Self>
{
}

// Just for example.
#[allow(dead_code)]
pub struct FixedSizeMatrix<T: MatrixElement, const ROWS: usize, const COLS: usize> {
    data: [[T; COLS]; ROWS],
}

impl<T: MatrixElement, const ROWS: usize, const COLS: usize> FixedSizeMatrix<T, ROWS, COLS> {
    #[allow(dead_code)]
    pub fn new() -> Self {
        Self {
            data: [[T::default(); COLS]; ROWS],
        }
    }

    #[allow(dead_code)]
    pub fn get(&self, row: usize, col: usize) -> T {
        self.data[row][col]
    }

    #[allow(dead_code)]
    pub fn set(&mut self, row: usize, col: usize, val: T) {
        self.data[row][col] = val
    }

    #[allow(dead_code)]
    pub fn from_vec(vec: Vec<Vec<T>>) -> Result<Self, String> {
        if vec.len() != ROWS {
            return Err(format!("Expected {} rows, got: {}", ROWS, vec.len()));
        }
        for (i, row) in vec.iter().enumerate() {
            if row.len() != COLS {
                return Err(format!(
                    "Row[{}]: Expected {} columns, got: {}",
                    i,
                    COLS,
                    row.len()
                ));
            }
        }

        let mut matrix = Self::new();

        for r in 0..ROWS {
            for c in 0..COLS {
                matrix.set(r, c, vec[r][c]);
            }
        }

        Ok(matrix)
    }
}

pub struct Matrix<T: MatrixElement> {
    rows: usize,
    cols: usize,
    data: Vec<T>,
}

impl<T: MatrixElement> Matrix<T> {
    #[allow(dead_code)]
    pub fn new(rows: usize, cols: usize) -> Self {
        Self {
            rows,
            cols,
            data: vec![T::default(); rows * cols],
        }
    }

    pub fn get(&self, row: usize, col: usize) -> T {
        self.data[row * self.cols + col]
    }

    pub fn set(&mut self, row: usize, col: usize, val: T) {
        self.data[row * self.cols + col] = val
    }

    pub fn rows(&self) -> usize {
        self.rows
    }

    pub fn cols(&self) -> usize {
        self.cols
    }

    pub fn from_vec(vec: Vec<Vec<T>>) -> Result<Self, String> {
        if vec.is_empty() {
            return Err("Matrix is empty".to_string());
        }

        let rows = vec.len();
        let cols = vec[0].len();

        if vec.iter().any(|row| row.len() != cols) {
            return Err("Matrix has to be rectangular".to_string());
        }

        let data: Vec<T> = vec.into_iter().flatten().collect();

        Ok(Self { rows, cols, data })
    }
}

pub trait IsIslandCell: IsBinary + MatrixElement {
    fn is_land(&self) -> bool;
    fn is_water(&self) -> bool;
    fn set_water(&mut self);
}

macro_rules! impl_is_island_cell {
    ($($t:ty), *) => {
        $(
            impl IsIslandCell for $t {
                fn is_land(&self) -> bool {
                    *self == 0
                }

                fn is_water(&self) -> bool {
                    *self == 1
                }

                fn set_water(&mut self){
                    *self =1
                }
            }
        )*
    };
}

impl_is_island_cell!(i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize);

pub type IslandCount = u32;

struct Solution;

impl Solution {
    const GRID_ROW_MIN_SIZE: usize = 1;
    const GRID_ROW_MAX_SIZE: usize = 100;
    const GRID_COL_MIN_SIZE: usize = 1;
    const GRID_COL_MAX_SIZE: usize = 100;

    pub fn closed_island<T: IsIslandCell>(grid: Vec<Vec<T>>) -> IslandCount {
        if !Self::_is_grid_valid(&grid) {
            return 0;
        }

        let mut matrix = Matrix::from_vec(grid).unwrap();

        let rows = matrix.rows();
        let cols = matrix.cols();

        let mut count: IslandCount = 0;

        // Step 1: Flood the land that touches the borders.
        for r in 0..rows {
            Self::_dfs(&mut matrix, r, 0); // Left border.
            Self::_dfs(&mut matrix, r, cols - 1); // Right border.
        }
        for c in 0..cols {
            Self::_dfs(&mut matrix, 0, c); // Upper border.
            Self::_dfs(&mut matrix, rows - 1, c); // Lower border
        }

        // Шаг 2: Count remain islands.
        for r in 0..rows {
            for c in 0..cols {
                if matrix.get(r, c).is_land() {
                    count += 1;
                    Self::_dfs(&mut matrix, r, c); // Flood the last island.
                }
            }
        }

        count
    }

    fn _dfs<T: IsIslandCell>(matrix: &mut Matrix<T>, r: usize, c: usize) {
        let rows = matrix.rows();
        let cols = matrix.cols();

        if r >= rows || c >= cols {
            return;
        }

        let mut cell = matrix.get(r, c);

        if cell.is_water() {
            return;
        }

        cell.set_water();
        matrix.set(r, c, cell);

        // Recursively traverse to the all 4 sides.
        if r > 0 {
            Self::_dfs(matrix, r - 1, c); // Up
        }
        if r < rows - 1 {
            Self::_dfs(matrix, r + 1, c); // Down
        }
        if c > 0 {
            Self::_dfs(matrix, r, c - 1); // Left
        }
        if c < cols - 1 {
            Self::_dfs(matrix, r, c + 1); // Right
        }
    }

    fn _is_grid_valid<T: IsIslandCell>(grid: &[Vec<T>]) -> bool {
        if grid.is_empty() {
            return false;
        }

        let rows = grid.len();
        let cols = grid[0].len();

        if rows < Self::GRID_ROW_MIN_SIZE || rows > Self::GRID_ROW_MAX_SIZE {
            return false;
        }
        if cols < Self::GRID_COL_MIN_SIZE || cols > Self::GRID_COL_MAX_SIZE {
            return false;
        }

        grid.iter()
            .all(|row| row.len() == cols && row.iter().all(|val| val.is_binary()))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    // Хелпер для удобного создания матрицы
    fn create_grid(rows: usize, cols: usize) -> Vec<Vec<i32>> {
        vec![vec![0; cols]; rows]
    }

    #[test]
    fn test_example_1() {
        let grid = vec![
            vec![1, 1, 1, 1, 1, 1, 1, 0],
            vec![1, 0, 0, 0, 0, 1, 1, 0],
            vec![1, 0, 1, 0, 1, 1, 1, 0],
            vec![1, 0, 0, 0, 0, 1, 0, 1],
            vec![1, 1, 1, 1, 1, 1, 1, 0],
        ];
        assert_eq!(Solution::closed_island(grid), 2);
    }

    #[test]
    fn test_example_2() {
        let grid = vec![
            vec![0, 0, 1, 0, 0],
            vec![0, 1, 0, 1, 0],
            vec![0, 1, 1, 1, 0],
        ];
        assert_eq!(Solution::closed_island(grid), 1);
    }

    #[test]
    fn test_example_3() {
        let grid = vec![
            vec![1, 1, 1, 1, 1, 1, 1],
            vec![1, 0, 0, 0, 0, 0, 1],
            vec![1, 0, 1, 1, 1, 0, 1],
            vec![1, 0, 1, 0, 1, 0, 1],
            vec![1, 0, 1, 1, 1, 0, 1],
            vec![1, 0, 0, 0, 0, 0, 1],
            vec![1, 1, 1, 1, 1, 1, 1],
        ];
        assert_eq!(Solution::closed_island(grid), 2);
    }

    #[test]
    fn test_1x1_water() {
        let grid = vec![vec![1]];
        assert_eq!(Solution::closed_island(grid), 0);
    }

    #[test]
    fn test_1x1_land() {
        // Земля касается границы, значит не закрыта
        let grid = vec![vec![0]];
        assert_eq!(Solution::closed_island(grid), 0);
    }

    #[test]
    fn test_all_water() {
        let grid = vec![vec![1, 1], vec![1, 1]];
        assert_eq!(Solution::closed_island(grid), 0);
    }

    #[test]
    fn test_all_land() {
        // Вся земля касается границ
        let grid = vec![vec![0, 0], vec![0, 0]];
        assert_eq!(Solution::closed_island(grid), 0);
    }

    #[test]
    fn test_single_closed_island_3x3() {
        let grid = vec![vec![1, 1, 1], vec![1, 0, 1], vec![1, 1, 1]];
        assert_eq!(Solution::closed_island(grid), 1);
    }

    #[test]
    fn test_two_closed_islands() {
        let grid = vec![
            vec![1, 1, 1, 1, 1],
            vec![1, 0, 1, 0, 1],
            vec![1, 1, 1, 1, 1],
        ];
        assert_eq!(Solution::closed_island(grid), 2);
    }

    #[test]
    fn test_land_on_top_border() {
        let grid = vec![vec![0, 1, 1], vec![1, 0, 1], vec![1, 1, 1]];
        assert_eq!(Solution::closed_island(grid), 1);
    }

    #[test]
    fn test_land_on_bottom_border() {
        let grid = vec![vec![1, 1, 1], vec![1, 0, 1], vec![0, 1, 1]];
        assert_eq!(Solution::closed_island(grid), 1);
    }

    #[test]
    fn test_land_on_left_border() {
        let grid = vec![vec![1, 1, 1], vec![0, 1, 1], vec![1, 1, 1]];
        assert_eq!(Solution::closed_island(grid), 0);
    }

    #[test]
    fn test_land_on_right_border() {
        let grid = vec![vec![1, 1, 1], vec![1, 1, 0], vec![1, 1, 1]];
        assert_eq!(Solution::closed_island(grid), 0);
    }

    #[test]
    fn test_land_connected_to_border_via_tunnel() {
        let grid = vec![
            vec![1, 0, 1, 1, 1],
            vec![1, 0, 0, 0, 1],
            vec![1, 1, 1, 0, 1],
            vec![1, 1, 1, 0, 1],
        ];
        assert_eq!(Solution::closed_island(grid), 0);
    }

    #[test]
    fn test_corner_island() {
        let grid = vec![vec![0, 1, 1, 1], vec![1, 0, 1, 1], vec![1, 1, 1, 1]];
        assert_eq!(Solution::closed_island(grid), 1);
    }

    #[test]
    fn test_nested_ring() {
        let grid = vec![
            vec![1, 1, 1, 1, 1],
            vec![1, 0, 0, 0, 1],
            vec![1, 0, 1, 0, 1],
            vec![1, 0, 0, 0, 1],
            vec![1, 1, 1, 1, 1],
        ];
        // Тут один большой остров-кольцо, но DFS сработает
        assert_eq!(Solution::closed_island(grid), 1);
    }

    #[test]
    fn test_many_separated_islands() {
        let grid = vec![
            vec![1, 1, 1, 1, 1],
            vec![1, 0, 1, 0, 1],
            vec![1, 1, 1, 0, 1],
            vec![1, 0, 1, 1, 1],
            vec![1, 1, 1, 1, 1],
        ];
        assert_eq!(Solution::closed_island(grid), 3);
    }

    #[test]
    fn test_5x5_border_water() {
        let grid = vec![
            vec![1, 1, 1, 1, 1],
            vec![1, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 1],
            vec![1, 1, 1, 1, 1],
        ];
        assert_eq!(Solution::closed_island(grid), 1);
    }

    #[test]
    fn test_10x10_no_islands() {
        let grid = vec![
            vec![1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            vec![1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
        ];
        assert_eq!(Solution::closed_island(grid), 1);
    }

    #[test]
    fn test_max_size_border() {
        // Тест на 100x100 (максимальный размер)
        let mut grid = create_grid(100, 100);

        // Заполняем всё водой
        for r in 0..100 {
            for c in 0..100 {
                grid[r][c] = 1;
            }
        }

        // Оставляем один остров в центре (закрытый)
        grid[50][50] = 0;

        assert_eq!(Solution::closed_island(grid), 1);
    }
}

// ================= SOLUTION FOR LEETCODE ======================= //

// use std::ops::{Add, Mul};

// pub trait IsBinary {
//     fn is_binary(&self) -> bool;
// }

// macro_rules! impl_is_binary {
//     ($($t:ty), *) => {
//         $(
//             impl IsBinary for $t {
//                 fn is_binary(&self) -> bool {
//                     *self == 0 || *self == 1
//                 }
//             }
//         )*
//     };
// }

// impl_is_binary!(i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize);

// pub trait MatrixElement:
//     Copy + Default + PartialEq + Add<Output = Self> + Mul<Output = Self>
// {
// }

// impl<T> MatrixElement for T where
//     T: Copy + Default + PartialEq + Add<Output = Self> + Mul<Output = Self>
// {
// }

// pub struct Matrix<T: MatrixElement> {
//     rows: usize,
//     cols: usize,
//     data: Vec<T>,
// }

// impl<T: MatrixElement> Matrix<T> {
//     pub fn get(&self, row: usize, col: usize) -> T {
//         self.data[row * self.cols + col]
//     }

//     pub fn set(&mut self, row: usize, col: usize, val: T) {
//         self.data[row * self.cols + col] = val
//     }

//     pub fn rows(&self) -> usize {
//         self.rows
//     }

//     pub fn cols(&self) -> usize {
//         self.cols
//     }

//     pub fn from_vec(vec: Vec<Vec<T>>) -> Result<Self, String> {
//         if vec.is_empty() {
//             return Err("Matrix is empty".to_string());
//         }

//         let rows = vec.len();
//         let cols = vec[0].len();

//         if vec.iter().any(|row| row.len() != cols) {
//             return Err("Matrix has to be rectangular".to_string());
//         }

//         let data: Vec<T> = vec.into_iter().flatten().collect();

//         Ok(Self { rows, cols, data })
//     }
// }

// pub trait IsIslandCell: IsBinary + MatrixElement {
//     fn is_land(&self) -> bool;
//     fn is_water(&self) -> bool;
//     fn set_water(&mut self);
// }

// macro_rules! impl_is_island_cell {
//     ($($t:ty), *) => {
//         $(
//             impl IsIslandCell for $t {
//                 fn is_land(&self) -> bool {
//                     *self == 0
//                 }

//                 fn is_water(&self) -> bool {
//                     *self == 1
//                 }

//                 fn set_water(&mut self){
//                     *self = 1
//                 }
//             }
//         )*
//     };
// }

// impl_is_island_cell!(i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize);

// impl Solution {
//     const GRID_ROW_MIN_SIZE: usize = 1;
//     const GRID_ROW_MAX_SIZE: usize = 100;
//     const GRID_COL_MIN_SIZE: usize = 1;
//     const GRID_COL_MAX_SIZE: usize = 100;

//     // Изменили возвращаемый тип на стандартный i32
//     pub fn closed_island(grid: Vec<Vec<i32>>) -> i32 {
//         if !Self::_is_grid_valid(&grid) {
//             return 0;
//         }

//         let mut matrix = Matrix::from_vec(grid).unwrap();

//         let rows = matrix.rows();
//         let cols = matrix.cols();

//         let mut count: i32 = 0;

//         // Step 1: Flood the land that touches the borders.
//         for r in 0..rows {
//             Self::_dfs(&mut matrix, r, 0); // Left border.
//             Self::_dfs(&mut matrix, r, cols - 1); // Right border.
//         }
//         for c in 0..cols {
//             Self::_dfs(&mut matrix, 0, c); // Upper border.
//             Self::_dfs(&mut matrix, rows - 1, c); // Lower border
//         }

//         // Step 2: Count remain islands.
//         for r in 0..rows {
//             for c in 0..cols {
//                 if matrix.get(r, c).is_land() {
//                     count += 1;
//                     Self::_dfs(&mut matrix, r, c); // Flood the last island.
//                 }
//             }
//         }

//         count
//     }

//     fn _dfs<T: IsIslandCell>(matrix: &mut Matrix<T>, r: usize, c: usize) {
//         let rows = matrix.rows();
//         let cols = matrix.cols();

//         if r >= rows || c >= cols {
//             return;
//         }

//         let mut cell = matrix.get(r, c);

//         if cell.is_water() {
//             return;
//         }

//         cell.set_water();
//         matrix.set(r, c, cell);

//         if r > 0 {
//             Self::_dfs(matrix, r - 1, c); // Up
//         }
//         if r < rows - 1 {
//             Self::_dfs(matrix, r + 1, c); // Down
//         }
//         if c > 0 {
//             Self::_dfs(matrix, r, c - 1); // Left
//         }
//         if c < cols - 1 {
//             Self::_dfs(matrix, r, c + 1); // Right
//         }
//     }

//     fn _is_grid_valid<T: IsIslandCell>(grid: &[Vec<T>]) -> bool {
//         if grid.is_empty() {
//             return false;
//         }

//         let rows = grid.len();
//         let cols = grid[0].len();

//         if rows < Self::GRID_ROW_MIN_SIZE || rows > Self::GRID_ROW_MAX_SIZE {
//             return false;
//         }
//         if cols < Self::GRID_COL_MIN_SIZE || cols > Self::GRID_COL_MAX_SIZE {
//             return false;
//         }

//         grid.iter()
//             .all(|row| row.len() == cols && row.iter().all(|val| val.is_binary()))
//     }
// }
