import XCTest
import SwiftUI
@testable import ColorFillingApp

class AdvancedDrawingToolsTests: XCTestCase {
    var drawingManager: DrawingManager!
    
    override func setUp() {
        super.setUp()
        drawingManager = DrawingManager()
    }
    
    override func tearDown() {
        drawingManager = nil
        super.tearDown()
    }
    
    // MARK: - Brush Settings Tests
    
    func testBrushSettingsInitialization() {
        let brushSettings = BrushSettings()
        
        XCTAssertEqual(brushSettings.size, 10.0, "Default brush size should be 10.0")
        XCTAssertEqual(brushSettings.opacity, 1.0, "Default brush opacity should be 1.0")
        XCTAssertEqual(brushSettings.hardness, 0.5, "Default brush hardness should be 0.5")
        XCTAssertEqual(brushSettings.spacing, 0.25, "Default brush spacing should be 0.25")
        XCTAssertEqual(brushSettings.flow, 1.0, "Default brush flow should be 1.0")
        XCTAssertTrue(brushSettings.smoothing, "Default smoothing should be enabled")
        XCTAssertTrue(brushSettings.pressureSensitivity, "Default pressure sensitivity should be enabled")
    }
    
    func testBrushSettingsCustomization() {
        var brushSettings = BrushSettings()
        
        brushSettings.size = 25.0
        brushSettings.opacity = 0.7
        brushSettings.hardness = 0.8
        brushSettings.spacing = 0.5
        brushSettings.flow = 0.6
        brushSettings.smoothing = false
        brushSettings.pressureSensitivity = false
        
        XCTAssertEqual(brushSettings.size, 25.0, "Brush size should be customizable")
        XCTAssertEqual(brushSettings.opacity, 0.7, "Brush opacity should be customizable")
        XCTAssertEqual(brushSettings.hardness, 0.8, "Brush hardness should be customizable")
        XCTAssertEqual(brushSettings.spacing, 0.5, "Brush spacing should be customizable")
        XCTAssertEqual(brushSettings.flow, 0.6, "Brush flow should be customizable")
        XCTAssertFalse(brushSettings.smoothing, "Smoothing should be customizable")
        XCTAssertFalse(brushSettings.pressureSensitivity, "Pressure sensitivity should be customizable")
    }
    
    func testBrushSettingsValidation() {
        var brushSettings = BrushSettings()
        
        // Test size bounds
        brushSettings.size = -5.0
        XCTAssertGreaterThanOrEqual(brushSettings.size, 1.0, "Brush size should not be negative")
        
        brushSettings.size = 1000.0
        XCTAssertLessThanOrEqual(brushSettings.size, 100.0, "Brush size should not exceed maximum")
        
        // Test opacity bounds
        brushSettings.opacity = -0.5
        XCTAssertGreaterThanOrEqual(brushSettings.opacity, 0.0, "Brush opacity should not be negative")
        
        brushSettings.opacity = 2.0
        XCTAssertLessThanOrEqual(brushSettings.opacity, 1.0, "Brush opacity should not exceed 1.0")
        
        // Test hardness bounds
        brushSettings.hardness = -0.5
        XCTAssertGreaterThanOrEqual(brushSettings.hardness, 0.0, "Brush hardness should not be negative")
        
        brushSettings.hardness = 2.0
        XCTAssertLessThanOrEqual(brushSettings.hardness, 1.0, "Brush hardness should not exceed 1.0")
    }
    
    // MARK: - Drawing Tool Tests
    
    func testDrawingToolSelection() {
        XCTAssertEqual(drawingManager.selectedTool, .brush, "Default tool should be brush")
        
        drawingManager.selectedTool = .pencil
        XCTAssertEqual(drawingManager.selectedTool, .pencil, "Tool selection should work")
        
        drawingManager.selectedTool = .eraser
        XCTAssertEqual(drawingManager.selectedTool, .eraser, "Eraser tool should be selectable")
        
        drawingManager.selectedTool = .marker
        XCTAssertEqual(drawingManager.selectedTool, .marker, "Marker tool should be selectable")
        
        drawingManager.selectedTool = .airbrush
        XCTAssertEqual(drawingManager.selectedTool, .airbrush, "Airbrush tool should be selectable")
        
        drawingManager.selectedTool = .highlighter
        XCTAssertEqual(drawingManager.selectedTool, .highlighter, "Highlighter tool should be selectable")
    }
    
    func testDrawingToolProperties() {
        // Test brush tool properties
        drawingManager.selectedTool = .brush
        XCTAssertEqual(drawingManager.brushSettings.size, 10.0, "Brush should have default size")
        
        // Test pencil tool properties
        drawingManager.selectedTool = .pencil
        XCTAssertEqual(drawingManager.brushSettings.size, 5.0, "Pencil should have smaller default size")
        
        // Test eraser tool properties
        drawingManager.selectedTool = .eraser
        XCTAssertEqual(drawingManager.brushSettings.size, 15.0, "Eraser should have larger default size")
        
        // Test marker tool properties
        drawingManager.selectedTool = .marker
        XCTAssertEqual(drawingManager.brushSettings.opacity, 0.8, "Marker should have reduced opacity")
        
        // Test airbrush tool properties
        drawingManager.selectedTool = .airbrush
        XCTAssertEqual(drawingManager.brushSettings.flow, 0.3, "Airbrush should have reduced flow")
        
        // Test highlighter tool properties
        drawingManager.selectedTool = .highlighter
        XCTAssertEqual(drawingManager.brushSettings.opacity, 0.4, "Highlighter should have low opacity")
    }
    
    // MARK: - Stroke Creation Tests
    
    func testStrokeCreation() {
        let startPoint = CGPoint(x: 10, y: 10)
        drawingManager.startStroke(at: startPoint)
        
        XCTAssertTrue(drawingManager.isDrawing, "Should be in drawing state")
        XCTAssertNotNil(drawingManager.currentStroke, "Current stroke should exist")
        XCTAssertEqual(drawingManager.currentStroke?.points.count, 1, "Stroke should have one point")
        XCTAssertEqual(drawingManager.currentStroke?.points.first, startPoint, "First point should match start point")
    }
    
    func testStrokePointAddition() {
        let startPoint = CGPoint(x: 10, y: 10)
        let secondPoint = CGPoint(x: 20, y: 20)
        
        drawingManager.startStroke(at: startPoint)
        drawingManager.addPoint(secondPoint)
        
        XCTAssertEqual(drawingManager.currentStroke?.points.count, 2, "Stroke should have two points")
        XCTAssertEqual(drawingManager.currentStroke?.points[1], secondPoint, "Second point should match")
    }
    
    func testStrokeCompletion() {
        let startPoint = CGPoint(x: 10, y: 10)
        let endPoint = CGPoint(x: 20, y: 20)
        
        drawingManager.startStroke(at: startPoint)
        drawingManager.addPoint(endPoint)
        drawingManager.endStroke()
        
        XCTAssertFalse(drawingManager.isDrawing, "Should not be in drawing state")
        XCTAssertNil(drawingManager.currentStroke, "Current stroke should be nil")
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should have one completed stroke")
        
        let completedStroke = drawingManager.strokes.first!
        XCTAssertEqual(completedStroke.points.count, 2, "Completed stroke should have two points")
        XCTAssertEqual(completedStroke.tool, drawingManager.selectedTool, "Stroke should have correct tool")
    }
    
    func testMultipleStrokes() {
        // Create first stroke
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.addPoint(CGPoint(x: 20, y: 20))
        drawingManager.endStroke()
        
        // Create second stroke
        drawingManager.startStroke(at: CGPoint(x: 30, y: 30))
        drawingManager.addPoint(CGPoint(x: 40, y: 40))
        drawingManager.endStroke()
        
        XCTAssertEqual(drawingManager.strokes.count, 2, "Should have two completed strokes")
        
        let firstStroke = drawingManager.strokes[0]
        let secondStroke = drawingManager.strokes[1]
        
        XCTAssertEqual(firstStroke.points.count, 2, "First stroke should have two points")
        XCTAssertEqual(secondStroke.points.count, 2, "Second stroke should have two points")
        XCTAssertNotEqual(firstStroke.id, secondStroke.id, "Strokes should have different IDs")
    }
    
    // MARK: - Color Management Tests
    
    func testColorSelection() {
        let testColor = Color.red
        drawingManager.selectedColor = testColor
        
        XCTAssertEqual(drawingManager.selectedColor, testColor, "Color selection should work")
    }
    
    func testColorHistory() {
        let colors = [Color.red, Color.blue, Color.green, Color.yellow]
        
        for color in colors {
            drawingManager.selectedColor = color
        }
        
        XCTAssertEqual(drawingManager.colorHistory.count, 4, "Color history should track all colors")
        XCTAssertEqual(drawingManager.colorHistory[0], colors[0], "First color should be red")
        XCTAssertEqual(drawingManager.colorHistory[1], colors[1], "Second color should be blue")
        XCTAssertEqual(drawingManager.colorHistory[2], colors[2], "Third color should be green")
        XCTAssertEqual(drawingManager.colorHistory[3], colors[3], "Fourth color should be yellow")
    }
    
    func testColorHistoryLimit() {
        // Add more colors than the history limit
        for i in 0..<60 {
            drawingManager.selectedColor = Color(hue: Double(i) / 60.0, saturation: 1.0, brightness: 1.0)
        }
        
        XCTAssertEqual(drawingManager.colorHistory.count, 50, "Color history should be limited to 50 colors")
    }
    
    // MARK: - Pressure Sensitivity Tests
    
    func testPressureSensitivity() {
        drawingManager.brushSettings.pressureSensitivity = true
        
        let startPoint = CGPoint(x: 10, y: 10)
        drawingManager.startStroke(at: startPoint)
        
        // Simulate pressure variation
        drawingManager.addPointWithPressure(CGPoint(x: 20, y: 20), pressure: 0.5)
        drawingManager.addPointWithPressure(CGPoint(x: 30, y: 30), pressure: 1.0)
        
        XCTAssertEqual(drawingManager.currentStroke?.points.count, 3, "Stroke should have three points")
        XCTAssertEqual(drawingManager.currentStroke?.pressures.count, 3, "Stroke should have three pressure values")
        XCTAssertEqual(drawingManager.currentStroke?.pressures[0], 1.0, "First pressure should be 1.0")
        XCTAssertEqual(drawingManager.currentStroke?.pressures[1], 0.5, "Second pressure should be 0.5")
        XCTAssertEqual(drawingManager.currentStroke?.pressures[2], 1.0, "Third pressure should be 1.0")
    }
    
    func testPressureSensitivityDisabled() {
        drawingManager.brushSettings.pressureSensitivity = false
        
        let startPoint = CGPoint(x: 10, y: 10)
        drawingManager.startStroke(at: startPoint)
        drawingManager.addPointWithPressure(CGPoint(x: 20, y: 20), pressure: 0.5)
        
        XCTAssertEqual(drawingManager.currentStroke?.pressures.count, 2, "Should have pressure values")
        XCTAssertEqual(drawingManager.currentStroke?.pressures[0], 1.0, "All pressures should be 1.0 when disabled")
        XCTAssertEqual(drawingManager.currentStroke?.pressures[1], 1.0, "All pressures should be 1.0 when disabled")
    }
    
    // MARK: - Stroke Smoothing Tests
    
    func testStrokeSmoothing() {
        drawingManager.brushSettings.smoothing = true
        
        let points = [
            CGPoint(x: 10, y: 10),
            CGPoint(x: 15, y: 12),
            CGPoint(x: 20, y: 15),
            CGPoint(x: 25, y: 18),
            CGPoint(x: 30, y: 20)
        ]
        
        drawingManager.startStroke(at: points[0])
        for i in 1..<points.count {
            drawingManager.addPoint(points[i])
        }
        drawingManager.endStroke()
        
        let stroke = drawingManager.strokes.first!
        XCTAssertEqual(stroke.points.count, 5, "Stroke should have all points")
        XCTAssertTrue(stroke.isSmoothed, "Stroke should be marked as smoothed")
    }
    
    func testStrokeSmoothingDisabled() {
        drawingManager.brushSettings.smoothing = false
        
        let points = [
            CGPoint(x: 10, y: 10),
            CGPoint(x: 15, y: 12),
            CGPoint(x: 20, y: 15)
        ]
        
        drawingManager.startStroke(at: points[0])
        for i in 1..<points.count {
            drawingManager.addPoint(points[i])
        }
        drawingManager.endStroke()
        
        let stroke = drawingManager.strokes.first!
        XCTAssertFalse(stroke.isSmoothed, "Stroke should not be marked as smoothed")
    }
    
    // MARK: - Drawing Presets Tests
    
    func testDrawingPresets() {
        let presets = DrawingPreset.allCases
        
        for preset in presets {
            drawingManager.applyPreset(preset)
            
            switch preset {
            case .fineLine:
                XCTAssertEqual(drawingManager.brushSettings.size, 2.0, "Fine line preset should have small size")
                XCTAssertEqual(drawingManager.brushSettings.opacity, 1.0, "Fine line preset should have full opacity")
            case .thickBrush:
                XCTAssertEqual(drawingManager.brushSettings.size, 25.0, "Thick brush preset should have large size")
                XCTAssertEqual(drawingManager.brushSettings.opacity, 0.8, "Thick brush preset should have reduced opacity")
            case .softAirbrush:
                XCTAssertEqual(drawingManager.brushSettings.size, 15.0, "Soft airbrush preset should have medium size")
                XCTAssertEqual(drawingManager.brushSettings.flow, 0.3, "Soft airbrush preset should have low flow")
            case .hardPencil:
                XCTAssertEqual(drawingManager.brushSettings.size, 5.0, "Hard pencil preset should have small size")
                XCTAssertEqual(drawingManager.brushSettings.hardness, 1.0, "Hard pencil preset should have full hardness")
            case .watercolor:
                XCTAssertEqual(drawingManager.brushSettings.size, 20.0, "Watercolor preset should have medium size")
                XCTAssertEqual(drawingManager.brushSettings.opacity, 0.6, "Watercolor preset should have reduced opacity")
            }
        }
    }
    
    // MARK: - Edge Cases Tests
    
    func testEmptyStroke() {
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.endStroke()
        
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should have one stroke")
        let stroke = drawingManager.strokes.first!
        XCTAssertEqual(stroke.points.count, 1, "Empty stroke should have one point")
    }
    
    func testSinglePointStroke() {
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.addPoint(CGPoint(x: 10, y: 10)) // Same point
        drawingManager.endStroke()
        
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should have one stroke")
        let stroke = drawingManager.strokes.first!
        XCTAssertEqual(stroke.points.count, 2, "Single point stroke should have two points")
    }
    
    func testRapidPointAddition() {
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        
        // Add many points rapidly
        for i in 1...100 {
            drawingManager.addPoint(CGPoint(x: 10 + i, y: 10 + i))
        }
        
        drawingManager.endStroke()
        
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should have one stroke")
        let stroke = drawingManager.strokes.first!
        XCTAssertEqual(stroke.points.count, 101, "Should have 101 points")
    }
    
    func testNegativeCoordinates() {
        drawingManager.startStroke(at: CGPoint(x: -10, y: -10))
        drawingManager.addPoint(CGPoint(x: -20, y: -20))
        drawingManager.endStroke()
        
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should handle negative coordinates")
        let stroke = drawingManager.strokes.first!
        XCTAssertEqual(stroke.points.count, 2, "Should have two points")
        XCTAssertEqual(stroke.points[0].x, -10, "Should preserve negative x coordinate")
        XCTAssertEqual(stroke.points[0].y, -10, "Should preserve negative y coordinate")
    }
    
    func testLargeCoordinates() {
        drawingManager.startStroke(at: CGPoint(x: 10000, y: 10000))
        drawingManager.addPoint(CGPoint(x: 20000, y: 20000))
        drawingManager.endStroke()
        
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should handle large coordinates")
        let stroke = drawingManager.strokes.first!
        XCTAssertEqual(stroke.points.count, 2, "Should have two points")
        XCTAssertEqual(stroke.points[0].x, 10000, "Should preserve large x coordinate")
        XCTAssertEqual(stroke.points[0].y, 10000, "Should preserve large y coordinate")
    }
    
    // MARK: - Performance Tests
    
    func testPerformanceLargeStroke() {
        measure {
            drawingManager.startStroke(at: CGPoint(x: 0, y: 0))
            
            for i in 1...1000 {
                drawingManager.addPoint(CGPoint(x: i, y: i))
            }
            
            drawingManager.endStroke()
        }
    }
    
    func testPerformanceMultipleStrokes() {
        measure {
            for strokeIndex in 0..<100 {
                drawingManager.startStroke(at: CGPoint(x: strokeIndex * 10, y: strokeIndex * 10))
                drawingManager.addPoint(CGPoint(x: strokeIndex * 10 + 5, y: strokeIndex * 10 + 5))
                drawingManager.endStroke()
            }
        }
    }
    
    func testPerformanceColorHistory() {
        measure {
            for i in 0..<1000 {
                drawingManager.selectedColor = Color(hue: Double(i) / 1000.0, saturation: 1.0, brightness: 1.0)
            }
        }
    }
}

// MARK: - Mock Objects for Testing

class MockDrawingManager: DrawingManager {
    var mockStrokes: [DrawingStroke] = []
    var mockCurrentStroke: DrawingStroke?
    var mockIsDrawing = false
    
    override var strokes: [DrawingStroke] {
        return mockStrokes
    }
    
    override var currentStroke: DrawingStroke? {
        return mockCurrentStroke
    }
    
    override var isDrawing: Bool {
        return mockIsDrawing
    }
    
    override func startStroke(at point: CGPoint) {
        mockIsDrawing = true
        mockCurrentStroke = DrawingStroke(
            id: UUID(),
            points: [point],
            pressures: [1.0],
            color: selectedColor,
            tool: selectedTool,
            brushSettings: brushSettings,
            timestamp: Date(),
            isSmoothed: brushSettings.smoothing
        )
    }
    
    override func addPoint(_ point: CGPoint) {
        guard var stroke = mockCurrentStroke else { return }
        stroke.points.append(point)
        stroke.pressures.append(1.0)
        mockCurrentStroke = stroke
    }
    
    override func endStroke() {
        if let stroke = mockCurrentStroke {
            mockStrokes.append(stroke)
        }
        mockCurrentStroke = nil
        mockIsDrawing = false
    }
}