import XCTest
import SwiftUI
@testable import ColorFillingApp

class SelectionToolsTests: XCTestCase {
    var selectionManager: SelectionManager!
    
    override func setUp() {
        super.setUp()
        selectionManager = SelectionManager()
    }
    
    override func tearDown() {
        selectionManager = nil
        super.tearDown()
    }
    
    // MARK: - Selection Tool Tests
    
    func testSelectionToolInitialization() {
        XCTAssertEqual(selectionManager.selectedTool, .rectangle, "Default selection tool should be rectangle")
    }
    
    func testSelectionToolSelection() {
        selectionManager.selectedTool = .ellipse
        XCTAssertEqual(selectionManager.selectedTool, .ellipse, "Ellipse tool should be selectable")
        
        selectionManager.selectedTool = .lasso
        XCTAssertEqual(selectionManager.selectedTool, .lasso, "Lasso tool should be selectable")
        
        selectionManager.selectedTool = .polygon
        XCTAssertEqual(selectionManager.selectedTool, .polygon, "Polygon tool should be selectable")
        
        selectionManager.selectedTool = .magicWand
        XCTAssertEqual(selectionManager.selectedTool, .magicWand, "Magic wand tool should be selectable")
        
        selectionManager.selectedTool = .quickSelection
        XCTAssertEqual(selectionManager.selectedTool, .quickSelection, "Quick selection tool should be selectable")
    }
    
    // MARK: - Rectangle Selection Tests
    
    func testRectangleSelectionCreation() {
        let startPoint = CGPoint(x: 10, y: 10)
        let endPoint = CGPoint(x: 50, y: 50)
        
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: startPoint)
        selectionManager.updateSelection(to: endPoint)
        selectionManager.endSelection()
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.tool, .rectangle, "Selection should have rectangle tool")
        XCTAssertEqual(selection.bounds.origin.x, 10, "Selection should have correct x origin")
        XCTAssertEqual(selection.bounds.origin.y, 10, "Selection should have correct y origin")
        XCTAssertEqual(selection.bounds.size.width, 40, "Selection should have correct width")
        XCTAssertEqual(selection.bounds.size.height, 40, "Selection should have correct height")
    }
    
    func testRectangleSelectionWithNegativeCoordinates() {
        let startPoint = CGPoint(x: 50, y: 50)
        let endPoint = CGPoint(x: 10, y: 10)
        
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: startPoint)
        selectionManager.updateSelection(to: endPoint)
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.bounds.origin.x, 10, "Selection should normalize x origin")
        XCTAssertEqual(selection.bounds.origin.y, 10, "Selection should normalize y origin")
        XCTAssertEqual(selection.bounds.size.width, 40, "Selection should have correct width")
        XCTAssertEqual(selection.bounds.size.height, 40, "Selection should have correct height")
    }
    
    // MARK: - Ellipse Selection Tests
    
    func testEllipseSelectionCreation() {
        let startPoint = CGPoint(x: 10, y: 10)
        let endPoint = CGPoint(x: 50, y: 50)
        
        selectionManager.selectedTool = .ellipse
        selectionManager.startSelection(at: startPoint)
        selectionManager.updateSelection(to: endPoint)
        selectionManager.endSelection()
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.tool, .ellipse, "Selection should have ellipse tool")
        XCTAssertEqual(selection.bounds.origin.x, 10, "Selection should have correct x origin")
        XCTAssertEqual(selection.bounds.origin.y, 10, "Selection should have correct y origin")
        XCTAssertEqual(selection.bounds.size.width, 40, "Selection should have correct width")
        XCTAssertEqual(selection.bounds.size.height, 40, "Selection should have correct height")
    }
    
    // MARK: - Lasso Selection Tests
    
    func testLassoSelectionCreation() {
        let points = [
            CGPoint(x: 10, y: 10),
            CGPoint(x: 30, y: 10),
            CGPoint(x: 30, y: 30),
            CGPoint(x: 10, y: 30)
        ]
        
        selectionManager.selectedTool = .lasso
        selectionManager.startSelection(at: points[0])
        
        for i in 1..<points.count {
            selectionManager.addLassoPoint(points[i])
        }
        
        selectionManager.endSelection()
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.tool, .lasso, "Selection should have lasso tool")
        XCTAssertEqual(selection.lassoPoints.count, 4, "Selection should have 4 lasso points")
    }
    
    func testLassoSelectionWithManyPoints() {
        selectionManager.selectedTool = .lasso
        selectionManager.startSelection(at: CGPoint(x: 0, y: 0))
        
        // Add many points to test performance
        for i in 1...100 {
            selectionManager.addLassoPoint(CGPoint(x: i, y: i))
        }
        
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.lassoPoints.count, 101, "Selection should have 101 lasso points")
    }
    
    // MARK: - Polygon Selection Tests
    
    func testPolygonSelectionCreation() {
        let points = [
            CGPoint(x: 10, y: 10),
            CGPoint(x: 30, y: 10),
            CGPoint(x: 30, y: 30),
            CGPoint(x: 10, y: 30)
        ]
        
        selectionManager.selectedTool = .polygon
        selectionManager.startSelection(at: points[0])
        
        for i in 1..<points.count {
            selectionManager.addPolygonPoint(points[i])
        }
        
        selectionManager.endSelection()
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.tool, .polygon, "Selection should have polygon tool")
        XCTAssertEqual(selection.polygonPoints.count, 4, "Selection should have 4 polygon points")
    }
    
    // MARK: - Magic Wand Selection Tests
    
    func testMagicWandSelection() {
        let point = CGPoint(x: 25, y: 25)
        let tolerance = 0.3
        
        selectionManager.selectedTool = .magicWand
        selectionManager.magicWandSelection(at: point, tolerance: tolerance)
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.tool, .magicWand, "Selection should have magic wand tool")
        XCTAssertEqual(selection.tolerance, tolerance, "Selection should have correct tolerance")
        XCTAssertEqual(selection.magicWandPoint, point, "Selection should have correct magic wand point")
    }
    
    // MARK: - Quick Selection Tests
    
    func testQuickSelection() {
        let point = CGPoint(x: 25, y: 25)
        
        selectionManager.selectedTool = .quickSelection
        selectionManager.quickSelection(at: point)
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.tool, .quickSelection, "Selection should have quick selection tool")
        XCTAssertEqual(selection.quickSelectionPoint, point, "Selection should have correct quick selection point")
    }
    
    // MARK: - Selection Mode Tests
    
    func testSelectionModeInitialization() {
        XCTAssertEqual(selectionManager.selectionMode, .replace, "Default selection mode should be replace")
    }
    
    func testSelectionModeSelection() {
        selectionManager.selectionMode = .add
        XCTAssertEqual(selectionManager.selectionMode, .add, "Add mode should be selectable")
        
        selectionManager.selectionMode = .subtract
        XCTAssertEqual(selectionManager.selectionMode, .subtract, "Subtract mode should be selectable")
        
        selectionManager.selectionMode = .intersect
        XCTAssertEqual(selectionManager.selectionMode, .intersect, "Intersect mode should be selectable")
    }
    
    func testAddSelectionMode() {
        // Create first selection
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 30, y: 30))
        selectionManager.endSelection()
        
        // Add second selection
        selectionManager.selectionMode = .add
        selectionManager.startSelection(at: CGPoint(x: 20, y: 20))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        XCTAssertEqual(selectionManager.selections.count, 2, "Should have two selections in add mode")
    }
    
    func testSubtractSelectionMode() {
        // Create first selection
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        // Subtract second selection
        selectionManager.selectionMode = .subtract
        selectionManager.startSelection(at: CGPoint(x: 20, y: 20))
        selectionManager.updateSelection(to: CGPoint(x: 40, y: 40))
        selectionManager.endSelection()
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection in subtract mode")
        
        let selection = selectionManager.selections.first!
        XCTAssertTrue(selection.hasSubtraction, "Selection should have subtraction")
        XCTAssertEqual(selection.subtractionBounds?.origin.x, 20, "Subtraction should have correct bounds")
    }
    
    func testIntersectSelectionMode() {
        // Create first selection
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        // Intersect second selection
        selectionManager.selectionMode = .intersect
        selectionManager.startSelection(at: CGPoint(x: 20, y: 20))
        selectionManager.updateSelection(to: CGPoint(x: 40, y: 40))
        selectionManager.endSelection()
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection in intersect mode")
        
        let selection = selectionManager.selections.first!
        XCTAssertTrue(selection.hasIntersection, "Selection should have intersection")
        XCTAssertEqual(selection.intersectionBounds?.origin.x, 20, "Intersection should have correct bounds")
    }
    
    // MARK: - Selection Management Tests
    
    func testClearAllSelections() {
        // Create multiple selections
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 30, y: 30))
        selectionManager.endSelection()
        
        selectionManager.startSelection(at: CGPoint(x: 40, y: 40))
        selectionManager.updateSelection(to: CGPoint(x: 60, y: 60))
        selectionManager.endSelection()
        
        XCTAssertEqual(selectionManager.selections.count, 2, "Should have two selections")
        
        selectionManager.clearAllSelections()
        
        XCTAssertEqual(selectionManager.selections.count, 0, "Should have no selections after clear")
    }
    
    func testRemoveSelection() {
        // Create selection
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 30, y: 30))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        
        selectionManager.removeSelection(selection)
        
        XCTAssertEqual(selectionManager.selections.count, 0, "Should have no selections after removal")
    }
    
    func testSelectAll() {
        let canvasSize = CGSize(width: 100, height: 100)
        
        selectionManager.selectAll(canvasSize: canvasSize)
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.bounds.origin.x, 0, "Select all should start at origin")
        XCTAssertEqual(selection.bounds.origin.y, 0, "Select all should start at origin")
        XCTAssertEqual(selection.bounds.size.width, 100, "Select all should have canvas width")
        XCTAssertEqual(selection.bounds.size.height, 100, "Select all should have canvas height")
    }
    
    func testInvertSelection() {
        let canvasSize = CGSize(width: 100, height: 100)
        
        // Create initial selection
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 30, y: 30))
        selectionManager.endSelection()
        
        selectionManager.invertSelection(canvasSize: canvasSize)
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection after invert")
        
        let selection = selectionManager.selections.first!
        XCTAssertTrue(selection.isInverted, "Selection should be marked as inverted")
    }
    
    // MARK: - Selection Properties Tests
    
    func testSelectionProperties() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        
        XCTAssertNotNil(selection.id, "Selection should have unique ID")
        XCTAssertNotNil(selection.timestamp, "Selection should have timestamp")
        XCTAssertEqual(selection.tool, .rectangle, "Selection should have correct tool")
        XCTAssertFalse(selection.isInverted, "Selection should not be inverted by default")
        XCTAssertFalse(selection.hasSubtraction, "Selection should not have subtraction by default")
        XCTAssertFalse(selection.hasIntersection, "Selection should not have intersection by default")
    }
    
    func testSelectionBoundsCalculation() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        
        XCTAssertEqual(selection.bounds.origin.x, 10, "Selection bounds should have correct x origin")
        XCTAssertEqual(selection.bounds.origin.y, 10, "Selection bounds should have correct y origin")
        XCTAssertEqual(selection.bounds.size.width, 40, "Selection bounds should have correct width")
        XCTAssertEqual(selection.bounds.size.height, 40, "Selection bounds should have correct height")
    }
    
    // MARK: - Selection Validation Tests
    
    func testSelectionValidation() {
        // Test valid selection
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        XCTAssertTrue(selectionManager.isValidSelection(selection), "Valid selection should pass validation")
    }
    
    func testSelectionValidationWithZeroSize() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 10, y: 10))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        XCTAssertFalse(selectionManager.isValidSelection(selection), "Zero-size selection should fail validation")
    }
    
    func testSelectionValidationWithNegativeSize() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 50, y: 50))
        selectionManager.updateSelection(to: CGPoint(x: 10, y: 10))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        XCTAssertTrue(selectionManager.isValidSelection(selection), "Negative size selection should be normalized and valid")
    }
    
    // MARK: - Selection Hit Testing Tests
    
    func testSelectionHitTesting() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        
        // Test point inside selection
        XCTAssertTrue(selectionManager.isPointInSelection(selection, point: CGPoint(x: 30, y: 30)), "Point inside selection should return true")
        
        // Test point outside selection
        XCTAssertFalse(selectionManager.isPointInSelection(selection, point: CGPoint(x: 5, y: 5)), "Point outside selection should return false")
        
        // Test point on selection edge
        XCTAssertTrue(selectionManager.isPointInSelection(selection, point: CGPoint(x: 10, y: 10)), "Point on selection edge should return true")
    }
    
    func testSelectionHitTestingWithEllipse() {
        selectionManager.selectedTool = .ellipse
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        
        // Test point inside ellipse
        XCTAssertTrue(selectionManager.isPointInSelection(selection, point: CGPoint(x: 30, y: 30)), "Point inside ellipse should return true")
        
        // Test point outside ellipse
        XCTAssertFalse(selectionManager.isPointInSelection(selection, point: CGPoint(x: 5, y: 5)), "Point outside ellipse should return false")
    }
    
    // MARK: - Selection Transformation Tests
    
    func testSelectionTransformation() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        let transform = CGAffineTransform(translationX: 10, y: 10)
        
        selectionManager.transformSelection(selection, transform: transform)
        
        XCTAssertEqual(selection.bounds.origin.x, 20, "Transformed selection should have correct x origin")
        XCTAssertEqual(selection.bounds.origin.y, 20, "Transformed selection should have correct y origin")
    }
    
    // MARK: - Edge Cases Tests
    
    func testSelectionWithExtremeCoordinates() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: -1000, y: -1000))
        selectionManager.updateSelection(to: CGPoint(x: 1000, y: 1000))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.bounds.origin.x, -1000, "Selection should handle extreme coordinates")
        XCTAssertEqual(selection.bounds.origin.y, -1000, "Selection should handle extreme coordinates")
        XCTAssertEqual(selection.bounds.size.width, 2000, "Selection should handle extreme coordinates")
        XCTAssertEqual(selection.bounds.size.height, 2000, "Selection should handle extreme coordinates")
    }
    
    func testSelectionWithVerySmallSize() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 10.1, y: 10.1))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.bounds.size.width, 0.1, "Selection should handle very small sizes")
        XCTAssertEqual(selection.bounds.size.height, 0.1, "Selection should handle very small sizes")
    }
    
    func testSelectionWithVeryLargeSize() {
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 0, y: 0))
        selectionManager.updateSelection(to: CGPoint(x: 10000, y: 10000))
        selectionManager.endSelection()
        
        let selection = selectionManager.selections.first!
        XCTAssertEqual(selection.bounds.size.width, 10000, "Selection should handle very large sizes")
        XCTAssertEqual(selection.bounds.size.height, 10000, "Selection should handle very large sizes")
    }
    
    // MARK: - Performance Tests
    
    func testPerformanceRectangleSelection() {
        measure {
            for _ in 0..<1000 {
                selectionManager.selectedTool = .rectangle
                selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
                selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
                selectionManager.endSelection()
            }
        }
    }
    
    func testPerformanceLassoSelection() {
        measure {
            selectionManager.selectedTool = .lasso
            selectionManager.startSelection(at: CGPoint(x: 0, y: 0))
            
            for i in 1...1000 {
                selectionManager.addLassoPoint(CGPoint(x: i, y: i))
            }
            
            selectionManager.endSelection()
        }
    }
    
    func testPerformanceSelectionHitTesting() {
        // Create many selections first
        for _ in 0..<100 {
            selectionManager.selectedTool = .rectangle
            selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
            selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
            selectionManager.endSelection()
        }
        
        measure {
            for selection in selectionManager.selections {
                _ = selectionManager.isPointInSelection(selection, point: CGPoint(x: 30, y: 30))
            }
        }
    }
}

// MARK: - Mock Objects for Testing

class MockSelectionManager: SelectionManager {
    var mockSelections: [Selection] = []
    var mockSelectedTool: SelectionTool = .rectangle
    var mockSelectionMode: SelectionMode = .replace
    
    override var selections: [Selection] {
        return mockSelections
    }
    
    override var selectedTool: SelectionTool {
        get { return mockSelectedTool }
        set { mockSelectedTool = newValue }
    }
    
    override var selectionMode: SelectionMode {
        get { return mockSelectionMode }
        set { mockSelectionMode = newValue }
    }
    
    override func startSelection(at point: CGPoint) {
        // Mock implementation
    }
    
    override func updateSelection(to point: CGPoint) {
        // Mock implementation
    }
    
    override func endSelection() {
        let selection = Selection(
            id: UUID(),
            tool: mockSelectedTool,
            bounds: CGRect(x: 10, y: 10, width: 40, height: 40),
            timestamp: Date()
        )
        mockSelections.append(selection)
    }
    
    override func clearAllSelections() {
        mockSelections.removeAll()
    }
    
    override func removeSelection(_ selection: Selection) {
        mockSelections.removeAll { $0.id == selection.id }
    }
}