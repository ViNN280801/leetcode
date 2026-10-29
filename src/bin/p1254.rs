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

use std::error::Error;
use std::fmt;
use std::ops::{Index, IndexMut};

// ============================ Matrix ============================ //

pub type Pos = (usize, usize);

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum MatrixError {
    Empty,
    Ragged {
        row: usize,
        expected: usize,
        found: usize,
    },
}

impl fmt::Display for MatrixError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Empty => write!(f, "matrix has no cells"),
            Self::Ragged {
                row,
                expected,
                found,
            } => write!(f, "row {row} has {found} columns, expected {expected}"),
        }
    }
}

impl Error for MatrixError {}

pub struct Matrix<T> {
    rows: usize,
    cols: usize,
    data: Vec<T>,
}

impl<T> Matrix<T> {
    pub fn from_rows(rows: Vec<Vec<T>>) -> Result<Self, MatrixError> {
        let cols = rows.first().map_or(0, Vec::len);

        if cols == 0 {
            return Err(MatrixError::Empty);
        }
        if let Some((row, found)) = rows
            .iter()
            .map(Vec::len)
            .enumerate()
            .find(|&(_, len)| len != cols)
        {
            return Err(MatrixError::Ragged {
                row,
                expected: cols,
                found,
            });
        }

        Ok(Self {
            rows: rows.len(),
            cols,
            data: rows.into_iter().flatten().collect(),
        })
    }

    pub fn rows(&self) -> usize {
        self.rows
    }

    pub fn cols(&self) -> usize {
        self.cols
    }

    pub fn contains(&self, (r, c): Pos) -> bool {
        r < self.rows && c < self.cols
    }

    /// Converts every element, keeping the shape. Stops at the first failure.
    pub fn try_map<U, E>(self, mut f: impl FnMut(Pos, T) -> Result<U, E>) -> Result<Matrix<U>, E> {
        let cols = self.cols;
        let data = self
            .data
            .into_iter()
            .enumerate()
            .map(|(i, val)| f((i / cols, i % cols), val))
            .collect::<Result<_, _>>()?;

        Ok(Matrix {
            rows: self.rows,
            cols,
            data,
        })
    }

    /// All positions, row by row.
    pub fn positions(&self) -> impl Iterator<Item = Pos> {
        let cols = self.cols;
        (0..self.rows).flat_map(move |r| (0..cols).map(move |c| (r, c)))
    }

    /// Positions on the outer edge. Corners are yielded twice.
    pub fn border(&self) -> impl Iterator<Item = Pos> + use<T> {
        let (rows, cols) = (self.rows, self.cols);
        let sides = (0..rows).flat_map(move |r| [(r, 0), (r, cols - 1)]);
        let top_bottom = (0..cols).flat_map(move |c| [(0, c), (rows - 1, c)]);
        sides.chain(top_bottom)
    }

    /// In-bounds 4-directional neighbours of `pos`.
    pub fn neighbors(&self, (r, c): Pos) -> impl Iterator<Item = Pos> {
        let (rows, cols) = (self.rows, self.cols);
        [
            r.checked_sub(1).map(|up| (up, c)),
            (r + 1 < rows).then(|| (r + 1, c)),
            c.checked_sub(1).map(|left| (r, left)),
            (c + 1 < cols).then(|| (r, c + 1)),
        ]
        .into_iter()
        .flatten()
    }
}

impl<T> Index<Pos> for Matrix<T> {
    type Output = T;

    fn index(&self, (r, c): Pos) -> &T {
        assert!(self.contains((r, c)), "({r}, {c}) is out of bounds");
        &self.data[r * self.cols + c]
    }
}

impl<T> IndexMut<Pos> for Matrix<T> {
    fn index_mut(&mut self, (r, c): Pos) -> &mut T {
        assert!(self.contains((r, c)), "({r}, {c}) is out of bounds");
        &mut self.data[r * self.cols + c]
    }
}

// ========================= Flood fill =========================== //

/// Template Method: `flood` is the fixed traversal, `can_enter` and `visit`
/// are the hooks each policy fills in.
pub trait GridFlood {
    type Cell;

    fn can_enter(&self, cell: &Self::Cell) -> bool;

    /// Must make the cell non-enterable, otherwise `flood` never terminates.
    fn visit(&mut self, cell: &mut Self::Cell);

    fn flood(&mut self, grid: &mut Matrix<Self::Cell>, start: Pos) {
        if !grid.contains(start) {
            return;
        }

        let mut stack = vec![start];

        while let Some(pos) = stack.pop() {
            let cell = &mut grid[pos];

            if !self.can_enter(cell) {
                continue;
            }

            self.visit(cell);
            stack.extend(grid.neighbors(pos));
        }
    }
}

// =========================== Islands ============================ //

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum Cell {
    Land,
    Water,
}

macro_rules! impl_cell_try_from {
    ($($t:ty),*) => {$(
        impl TryFrom<$t> for Cell {
            type Error = $t;
            fn try_from(v: $t) -> Result<Self, Self::Error> {
                match v {
                    0 => Ok(Cell::Land),
                    1 => Ok(Cell::Water),
                    other => Err(other),
                }
            }
        }
    )*};
}

impl_cell_try_from!(i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize);

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum GridError {
    Shape(MatrixError),
    InvalidCell { row: usize, col: usize },
}

impl fmt::Display for GridError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Shape(err) => write!(f, "invalid grid shape: {err}"),
            Self::InvalidCell { row, col } => {
                write!(f, "cell ({row}, {col}) is neither 0 (land) nor 1 (water)")
            }
        }
    }
}

impl Error for GridError {
    fn source(&self) -> Option<&(dyn Error + 'static)> {
        match self {
            Self::Shape(err) => Some(err),
            Self::InvalidCell { .. } => None,
        }
    }
}

impl From<MatrixError> for GridError {
    fn from(err: MatrixError) -> Self {
        Self::Shape(err)
    }
}

struct SinkLand;

impl GridFlood for SinkLand {
    type Cell = Cell;

    fn can_enter(&self, cell: &Cell) -> bool {
        *cell == Cell::Land
    }

    fn visit(&mut self, cell: &mut Cell) {
        *cell = Cell::Water
    }
}

pub type IslandCount = u32;

/// A validated grid of land and water.
pub struct IslandMap {
    cells: Matrix<Cell>,
}

impl IslandMap {
    /// Consumes the map: counting sinks every island it visits.
    pub fn count_closed_islands(mut self) -> IslandCount {
        let mut sink = SinkLand;

        // Islands touching the edge are not closed — sink them first.
        for pos in self.cells.border() {
            sink.flood(&mut self.cells, pos);
        }

        let mut count = 0;

        for pos in self.cells.positions() {
            if self.cells[pos] == Cell::Land {
                count += 1;
                sink.flood(&mut self.cells, pos);
            }
        }

        count
    }
}

impl<T> TryFrom<Vec<Vec<T>>> for IslandMap
where
    Cell: TryFrom<T>,
{
    type Error = GridError;

    fn try_from(grid: Vec<Vec<T>>) -> Result<Self, GridError> {
        let cells = Matrix::from_rows(grid)?.try_map(|(row, col), val| {
            Cell::try_from(val).map_err(|_| GridError::InvalidCell { row, col })
        })?;

        Ok(Self { cells })
    }
}

// ======================= LeetCode adapter ======================= //

pub struct Solution;

impl Solution {
    pub fn closed_island(grid: Vec<Vec<i32>>) -> i32 {
        let map = IslandMap::try_from(grid).expect("LeetCode guarantees a valid 0/1 grid");
        i32::try_from(map.count_closed_islands()).expect("at most 100x100 cells")
    }
}

fn main() {
    let grid = vec![
        vec![1, 1, 1, 1, 1, 1, 1, 0],
        vec![1, 0, 0, 0, 0, 1, 1, 0],
        vec![1, 0, 1, 0, 1, 1, 1, 0],
        vec![1, 0, 0, 0, 0, 1, 0, 1],
        vec![1, 1, 1, 1, 1, 1, 1, 0],
    ];
    println!("{}", Solution::closed_island(grid));

    match IslandMap::try_from(vec![vec![0, 2], vec![1, 1]]) {
        Ok(map) => println!("{}", map.count_closed_islands()),
        Err(err) => println!("error: {err}"),
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

    #[test]
    fn test_max_size_snake() {
        // Змейка: чётные строки целиком суша, нечётные — вода с одним проходом,
        // чередующимся справа и слева. Один остров ~5000 клеток, касается границы.
        let mut grid = vec![vec![1; 100]; 100];

        for (r, row) in grid.iter_mut().enumerate() {
            if r % 2 == 0 {
                row.fill(0);
            } else if r % 4 == 1 {
                row[99] = 0;
            } else {
                row[0] = 0;
            }
        }

        assert_eq!(Solution::closed_island(grid), 0);
    }

    #[test]
    fn test_error_empty_grid() {
        let grid: Vec<Vec<i32>> = vec![];
        assert_eq!(
            IslandMap::try_from(grid).err(),
            Some(GridError::Shape(MatrixError::Empty))
        );
    }

    #[test]
    fn test_error_empty_row() {
        let grid: Vec<Vec<i32>> = vec![vec![]];
        assert_eq!(
            IslandMap::try_from(grid).err(),
            Some(GridError::Shape(MatrixError::Empty))
        );
    }

    #[test]
    fn test_error_ragged_grid() {
        let grid = vec![vec![1, 1, 1], vec![1, 0], vec![1, 1, 1]];
        assert_eq!(
            IslandMap::try_from(grid).err(),
            Some(GridError::Shape(MatrixError::Ragged {
                row: 1,
                expected: 3,
                found: 2
            }))
        );
    }

    #[test]
    fn test_error_invalid_cell() {
        let grid = vec![vec![1, 1, 1], vec![1, 0, 1], vec![1, 2, 1]];
        assert_eq!(
            IslandMap::try_from(grid).err(),
            Some(GridError::InvalidCell { row: 2, col: 1 })
        );
    }

    #[test]
    fn test_other_integer_types() {
        let grid: Vec<Vec<u8>> = vec![vec![1, 1, 1], vec![1, 0, 1], vec![1, 1, 1]];
        assert_eq!(IslandMap::try_from(grid).unwrap().count_closed_islands(), 1);
    }
}
