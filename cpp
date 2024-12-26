// Tetra Turns - A Block Puzzle Game

// Canvas setup
const canvas = document.createElement('canvas');
canvas.width = 301;
canvas.height = 601;
document.body.appendChild(canvas);
const ctx = canvas.getContext('2d');

// Game variables
const gridWidth = 10;
const gridHeight = 20;
const blockSize = 30;
const grid = Array.from({ length: gridHeight }, () => Array(gridWidth).fill(0));
let currentPiece = null;
let score = 0;
let gameInterval = null;

// Tetris shapes
const shapes = [
  [[1, 1, 1], [0, 1, 0]], // T
  [[1, 1], [1, 1]],       // O
  [[1, 1, 0], [0, 1, 1]], // Z
  [[0, 1, 1], [1, 1, 0]], // S
  [[1, 1, 1, 1]],         // I
  [[1, 1, 1], [1, 0, 0]], // L
  [[1, 1, 1], [0, 0, 1]]  // J
];

// Utility functions
function drawGrid() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  for (let y = 0; y < gridHeight; y++) {
    for (let x = 0; x < gridWidth; x++) {
      if (grid[y][x]) {
        ctx.fillStyle = 'blue';
        ctx.fillRect(x * blockSize, y * blockSize, blockSize, blockSize);
      }
    }
  }
}

function drawPiece(piece) {
  ctx.fillStyle = 'red';
  for (let y = 0; y < piece.shape.length; y++) {
    for (let x = 0; x < piece.shape[y].length; x++) {
      if (piece.shape[y][x]) {
        ctx.fillRect(
          (piece.x + x) * blockSize,
          (piece.y + y) * blockSize,
          blockSize,
          blockSize
        );
      }
    }
  }
}

function createPiece() {
  const shape = shapes[Math.floor(Math.random() * shapes.length)];
  return {
    shape,
    x: Math.floor(gridWidth / 2) - Math.floor(shape[0].length / 2),
    y: 0
  };
}

function rotatePiece(piece) {
  const newShape = piece.shape[0].map((_, index) =>
    piece.shape.map(row => row[index]).reverse()
  );
  piece.shape = newShape;
}

function canPlacePiece(piece) {
  for (let y = 0; y < piece.shape.length; y++) {
    for (let x = 0; x < piece.shape[y].length; x++) {
      if (
        piece.shape[y][x] &&
        (grid[piece.y + y] === undefined ||
          grid[piece.y + y][piece.x + x] === undefined ||
          grid[piece.y + y][piece.x + x])
      ) {
        return false;
      }
    }
  }
  return true;
}

function mergePiece(piece) {
  for (let y = 0; y < piece.shape.length; y++) {
    for (let x = 0; x < piece.shape[y].length; x++) {
      if (piece.shape[y][x]) {
        grid[piece.y + y][piece.x + x] = 1;
      }
    }
  }
}

function clearLines() {
  let linesCleared = 0;
  for (let y = gridHeight - 1; y >= 0; y--) {
    if (grid[y].every(cell => cell)) {
      grid.splice(y, 1);
      grid.unshift(Array(gridWidth).fill(0));
      linesCleared++;
    }
  }
  score += linesCleared * 100;
}

function gameLoop() {
  if (!currentPiece) {
    currentPiece = createPiece();
    if (!canPlacePiece(currentPiece)) {
      clearInterval(gameInterval);
      alert(`Game Over! Your score: ${score}`);
      return;
    }
  }

  currentPiece.y++;

  if (!canPlacePiece(currentPiece)) {
    currentPiece.y--;
    mergePiece(currentPiece);
    clearLines();
    currentPiece = null;
  }

  drawGrid();
  if (currentPiece) drawPiece(currentPiece);
}

function randomRotation() {
  if (currentPiece) {
    rotatePiece(currentPiece);
    if (!canPlacePiece(currentPiece)) rotatePiece(currentPiece); // Undo rotation
  }
}

// Event listeners
window.addEventListener('keydown', e => {
  if (!currentPiece) return;
  if (e.key === 'ArrowLeft') {
    currentPiece.x--;
    if (!canPlacePiece(currentPiece)) currentPiece.x++;
  } else if (e.key === 'ArrowRight') {
    currentPiece.x++;
    if (!canPlacePiece(currentPiece)) currentPiece.x--;
  } else if (e.key === 'ArrowDown') {
    gameLoop();
  } else if (e.key === ' ') {
    rotatePiece(currentPiece);
    if (!canPlacePiece(currentPiece)) rotatePiece(currentPiece); // Undo rotation
  }
});

// Start the game
gameInterval = setInterval(gameLoop, 500);
setInterval(randomRotation, 5000);
