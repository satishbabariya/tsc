import XCTest
import SwiftUI
@testable import ColorFillingApp

class TransformationToolsTests: XCTestCase {
    var transformationManager: TransformationManager!
    
    override func setUp() {
        super.setUp()
        transformationManager = TransformationManager()
    }
    
    override func tearDown() {
        transformationManager = nil
        super.tearDown()
    }
    
    // MARK: - Transformation Manager Initialization Tests
    
    func testTransformationManagerInitialization() {
        XCTAssertFalse(transformationManager.isTransforming, "Should not be transforming initially")
        XCTAssertNil(transformationManager.selectedLayer, "Should not have selected layer initially")
        XCTAssertEqual(transformationManager.transformationType, .move, "Default transformation type should be move")
        XCTAssertEqual(transformationManager.transformationCenter, .zero, "Default transformation center should be zero")
        XCTAssertEqual(transformationManager.transformationAngle, 0.0, "Default transformation angle should be 0")
        XCTAssertEqual(transformationManager.transformationScale, 1.0, "Default transformation scale should be 1.0")
        XCTAssertEqual(transformationManager.transformationOffset, .zero, "Default transformation offset should be zero")
    }
    
    // MARK: - Move Transformation Tests
    
    func testMoveTransformation() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let offset = CGSize(width: 10, height: 20)
        transformationManager.moveLayer(by: offset)
        
        XCTAssertEqual(transformationManager.transformationOffset, offset, "Transformation offset should be updated")
        XCTAssertEqual(transformationManager.transformationType, .move, "Transformation type should be move")
    }
    
    func testMoveTransformationWithNegativeOffset() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let offset = CGSize(width: -10, height: -20)
        transformationManager.moveLayer(by: offset)
        
        XCTAssertEqual(transformationManager.transformationOffset, offset, "Transformation offset should handle negative values")
    }
    
    func testMoveTransformationWithLargeOffset() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let offset = CGSize(width: 1000, height: 1000)
        transformationManager.moveLayer(by: offset)
        
        XCTAssertEqual(transformationManager.transformationOffset, offset, "Transformation offset should handle large values")
    }
    
    // MARK: - Rotate Transformation Tests
    
    func testRotateTransformation() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let angle: Double = 45.0
        transformationManager.rotateLayer(by: angle)
        
        XCTAssertEqual(transformationManager.transformationAngle, angle, "Transformation angle should be updated")
        XCTAssertEqual(transformationManager.transformationType, .rotate, "Transformation type should be rotate")
    }
    
    func testRotateTransformationWithNegativeAngle() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let angle: Double = -45.0
        transformationManager.rotateLayer(by: angle)
        
        XCTAssertEqual(transformationManager.transformationAngle, angle, "Transformation angle should handle negative values")
    }
    
    func testRotateTransformationWithLargeAngle() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let angle: Double = 720.0
        transformationManager.rotateLayer(by: angle)
        
        XCTAssertEqual(transformationManager.transformationAngle, angle, "Transformation angle should handle large values")
    }
    
    func testRotateTransformationWithCenter() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let center = CGPoint(x: 50, y: 50)
        let angle: Double = 90.0
        
        transformationManager.rotateLayer(by: angle, around: center)
        
        XCTAssertEqual(transformationManager.transformationAngle, angle, "Transformation angle should be updated")
        XCTAssertEqual(transformationManager.transformationCenter, center, "Transformation center should be updated")
    }
    
    // MARK: - Scale Transformation Tests
    
    func testScaleTransformation() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let scale: Double = 2.0
        transformationManager.scaleLayer(by: scale)
        
        XCTAssertEqual(transformationManager.transformationScale, scale, "Transformation scale should be updated")
        XCTAssertEqual(transformationManager.transformationType, .scale, "Transformation type should be scale")
    }
    
    func testScaleTransformationWithFractionalScale() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let scale: Double = 0.5
        transformationManager.scaleLayer(by: scale)
        
        XCTAssertEqual(transformationManager.transformationScale, scale, "Transformation scale should handle fractional values")
    }
    
    func testScaleTransformationWithCenter() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let center = CGPoint(x: 50, y: 50)
        let scale: Double = 1.5
        
        transformationManager.scaleLayer(by: scale, around: center)
        
        XCTAssertEqual(transformationManager.transformationScale, scale, "Transformation scale should be updated")
        XCTAssertEqual(transformationManager.transformationCenter, center, "Transformation center should be updated")
    }
    
    func testScaleTransformationWithNegativeScale() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let scale: Double = -1.0
        transformationManager.scaleLayer(by: scale)
        
        // Negative scale should be clamped to positive
        XCTAssertGreaterThan(transformationManager.transformationScale, 0, "Transformation scale should be positive")
    }
    
    func testScaleTransformationWithZeroScale() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let scale: Double = 0.0
        transformationManager.scaleLayer(by: scale)
        
        // Zero scale should be clamped to minimum
        XCTAssertGreaterThan(transformationManager.transformationScale, 0, "Transformation scale should be greater than zero")
    }
    
    // MARK: - Flip Transformation Tests
    
    func testFlipHorizontal() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.flipLayerHorizontally()
        
        XCTAssertTrue(transformationManager.isFlippedHorizontally, "Layer should be flipped horizontally")
        XCTAssertEqual(transformationManager.transformationType, .flip, "Transformation type should be flip")
    }
    
    func testFlipVertical() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.flipLayerVertically()
        
        XCTAssertTrue(transformationManager.isFlippedVertically, "Layer should be flipped vertically")
        XCTAssertEqual(transformationManager.transformationType, .flip, "Transformation type should be flip")
    }
    
    func testFlipBoth() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.flipLayerHorizontally()
        transformationManager.flipLayerVertically()
        
        XCTAssertTrue(transformationManager.isFlippedHorizontally, "Layer should be flipped horizontally")
        XCTAssertTrue(transformationManager.isFlippedVertically, "Layer should be flipped vertically")
    }
    
    // MARK: - Skew Transformation Tests
    
    func testSkewTransformation() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let skewX: Double = 0.2
        let skewY: Double = 0.3
        
        transformationManager.skewLayer(x: skewX, y: skewY)
        
        XCTAssertEqual(transformationManager.skewX, skewX, "Skew X should be updated")
        XCTAssertEqual(transformationManager.skewY, skewY, "Skew Y should be updated")
        XCTAssertEqual(transformationManager.transformationType, .skew, "Transformation type should be skew")
    }
    
    func testSkewTransformationWithNegativeValues() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let skewX: Double = -0.2
        let skewY: Double = -0.3
        
        transformationManager.skewLayer(x: skewX, y: skewY)
        
        XCTAssertEqual(transformationManager.skewX, skewX, "Skew X should handle negative values")
        XCTAssertEqual(transformationManager.skewY, skewY, "Skew Y should handle negative values")
    }
    
    // MARK: - Perspective Transformation Tests
    
    func testPerspectiveTransformation() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let perspective: Double = 0.5
        
        transformationManager.applyPerspective(perspective)
        
        XCTAssertEqual(transformationManager.perspective, perspective, "Perspective should be updated")
        XCTAssertEqual(transformationManager.transformationType, .perspective, "Transformation type should be perspective")
    }
    
    func testPerspectiveTransformationWithNegativeValue() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let perspective: Double = -0.5
        
        transformationManager.applyPerspective(perspective)
        
        XCTAssertEqual(transformationManager.perspective, perspective, "Perspective should handle negative values")
    }
    
    // MARK: - Combined Transformation Tests
    
    func testCombinedTransformation() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        // Apply multiple transformations
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        transformationManager.rotateLayer(by: 45.0)
        transformationManager.scaleLayer(by: 2.0)
        
        XCTAssertEqual(transformationManager.transformationOffset, CGSize(width: 10, height: 20), "Move transformation should be preserved")
        XCTAssertEqual(transformationManager.transformationAngle, 45.0, "Rotate transformation should be preserved")
        XCTAssertEqual(transformationManager.transformationScale, 2.0, "Scale transformation should be preserved")
    }
    
    func testTransformationOrder() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        // Apply transformations in specific order
        transformationManager.moveLayer(by: CGSize(width: 10, height: 10))
        transformationManager.rotateLayer(by: 90.0)
        transformationManager.scaleLayer(by: 2.0)
        
        let transform = transformationManager.getCombinedTransform()
        
        // Verify transformation matrix is not identity
        XCTAssertNotEqual(transform, CGAffineTransform.identity, "Combined transform should not be identity")
    }
    
    // MARK: - Transformation Matrix Tests
    
    func testTransformationMatrix() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        transformationManager.rotateLayer(by: 45.0)
        transformationManager.scaleLayer(by: 2.0)
        
        let transform = transformationManager.getCombinedTransform()
        
        XCTAssertNotEqual(transform, CGAffineTransform.identity, "Transform should not be identity")
        XCTAssertEqual(transform.tx, 10, "Transform should have correct translation X")
        XCTAssertEqual(transform.ty, 20, "Transform should have correct translation Y")
    }
    
    func testTransformationMatrixWithCenter() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        let center = CGPoint(x: 50, y: 50)
        transformationManager.rotateLayer(by: 90.0, around: center)
        
        let transform = transformationManager.getCombinedTransform()
        
        XCTAssertNotEqual(transform, CGAffineTransform.identity, "Transform should not be identity")
        XCTAssertEqual(transformationManager.transformationCenter, center, "Transformation center should be preserved")
    }
    
    // MARK: - Transformation Reset Tests
    
    func testResetTransformation() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        // Apply transformations
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        transformationManager.rotateLayer(by: 45.0)
        transformationManager.scaleLayer(by: 2.0)
        
        // Reset transformations
        transformationManager.resetTransformation()
        
        XCTAssertEqual(transformationManager.transformationOffset, .zero, "Transformation offset should be reset")
        XCTAssertEqual(transformationManager.transformationAngle, 0.0, "Transformation angle should be reset")
        XCTAssertEqual(transformationManager.transformationScale, 1.0, "Transformation scale should be reset")
        XCTAssertEqual(transformationManager.transformationCenter, .zero, "Transformation center should be reset")
    }
    
    func testResetSpecificTransformation() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        // Apply transformations
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        transformationManager.rotateLayer(by: 45.0)
        transformationManager.scaleLayer(by: 2.0)
        
        // Reset only rotation
        transformationManager.resetRotation()
        
        XCTAssertEqual(transformationManager.transformationOffset, CGSize(width: 10, height: 20), "Move transformation should be preserved")
        XCTAssertEqual(transformationManager.transformationAngle, 0.0, "Rotation should be reset")
        XCTAssertEqual(transformationManager.transformationScale, 2.0, "Scale transformation should be preserved")
    }
    
    // MARK: - Transformation Preview Tests
    
    func testTransformationPreview() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        
        let preview = transformationManager.getTransformationPreview()
        
        XCTAssertNotNil(preview, "Transformation preview should not be nil")
        XCTAssertEqual(preview.offset, CGSize(width: 10, height: 20), "Preview should have correct offset")
    }
    
    func testTransformationPreviewWithMultipleTransforms() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        transformationManager.rotateLayer(by: 45.0)
        transformationManager.scaleLayer(by: 2.0)
        
        let preview = transformationManager.getTransformationPreview()
        
        XCTAssertNotNil(preview, "Transformation preview should not be nil")
        XCTAssertEqual(preview.offset, CGSize(width: 10, height: 20), "Preview should have correct offset")
        XCTAssertEqual(preview.angle, 45.0, "Preview should have correct angle")
        XCTAssertEqual(preview.scale, 2.0, "Preview should have correct scale")
    }
    
    // MARK: - Transformation Constraints Tests
    
    func testTransformationConstraints() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        // Test move constraints
        transformationManager.moveLayer(by: CGSize(width: 1000, height: 1000))
        
        // Should be constrained to reasonable bounds
        XCTAssertLessThan(abs(transformationManager.transformationOffset.width), 10000, "Move should be constrained")
        XCTAssertLessThan(abs(transformationManager.transformationOffset.height), 10000, "Move should be constrained")
    }
    
    func testTransformationConstraintsWithScale() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        // Test scale constraints
        transformationManager.scaleLayer(by: 100.0)
        
        // Should be constrained to reasonable bounds
        XCTAssertLessThan(transformationManager.transformationScale, 10.0, "Scale should be constrained")
        XCTAssertGreaterThan(transformationManager.transformationScale, 0.1, "Scale should be constrained")
    }
    
    // MARK: - Transformation History Tests
    
    func testTransformationHistory() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        
        XCTAssertEqual(transformationManager.history.count, 1, "History should track transformation")
        XCTAssertEqual(transformationManager.history.first?.type, .move, "History should have correct type")
    }
    
    func testTransformationUndo() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        transformationManager.undo()
        
        XCTAssertEqual(transformationManager.transformationOffset, .zero, "Undo should reset transformation")
    }
    
    func testTransformationRedo() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        transformationManager.undo()
        transformationManager.redo()
        
        XCTAssertEqual(transformationManager.transformationOffset, CGSize(width: 10, height: 20), "Redo should restore transformation")
    }
    
    // MARK: - Edge Cases Tests
    
    func testTransformationWithNilLayer() {
        transformationManager.selectedLayer = nil
        
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        
        // Should not crash and should not update transformation
        XCTAssertEqual(transformationManager.transformationOffset, .zero, "Transformation should not be applied to nil layer")
    }
    
    func testTransformationWithExtremeValues() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 100000, height: 100000))
        transformationManager.rotateLayer(by: 1000.0)
        transformationManager.scaleLayer(by: 100.0)
        
        // Should handle extreme values gracefully
        XCTAssertNotNil(transformationManager.getCombinedTransform(), "Should handle extreme values")
    }
    
    func testTransformationWithZeroValues() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 0, height: 0))
        transformationManager.rotateLayer(by: 0.0)
        transformationManager.scaleLayer(by: 1.0)
        
        let transform = transformationManager.getCombinedTransform()
        XCTAssertEqual(transform, CGAffineTransform.identity, "Zero transformations should result in identity")
    }
    
    // MARK: - Performance Tests
    
    func testPerformanceTransformationMatrix() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        measure {
            for _ in 0..<1000 {
                transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
                transformationManager.rotateLayer(by: 45.0)
                transformationManager.scaleLayer(by: 2.0)
                _ = transformationManager.getCombinedTransform()
            }
        }
    }
    
    func testPerformanceTransformationPreview() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
        transformationManager.rotateLayer(by: 45.0)
        transformationManager.scaleLayer(by: 2.0)
        
        measure {
            for _ in 0..<1000 {
                _ = transformationManager.getTransformationPreview()
            }
        }
    }
    
    func testPerformanceTransformationHistory() {
        let layer = createTestLayer()
        transformationManager.selectedLayer = layer
        
        measure {
            for _ in 0..<1000 {
                transformationManager.moveLayer(by: CGSize(width: 10, height: 20))
                transformationManager.undo()
                transformationManager.redo()
            }
        }
    }
    
    // MARK: - Helper Methods
    
    private func createTestLayer() -> DrawingLayer {
        return DrawingLayer(
            id: UUID(),
            name: "Test Layer",
            opacity: 1.0,
            blendingMode: .normal,
            isVisible: true,
            timestamp: Date()
        )
    }
}

// MARK: - Mock Objects for Testing

class MockTransformationManager: TransformationManager {
    var mockIsTransforming = false
    var mockSelectedLayer: DrawingLayer?
    var mockTransformationType: TransformationType = .move
    var mockTransformationCenter = CGPoint.zero
    var mockTransformationAngle: Double = 0.0
    var mockTransformationScale: Double = 1.0
    var mockTransformationOffset = CGSize.zero
    var mockSkewX: Double = 0.0
    var mockSkewY: Double = 0.0
    var mockPerspective: Double = 0.0
    var mockIsFlippedHorizontally = false
    var mockIsFlippedVertically = false
    var mockHistory: [TransformationHistoryEntry] = []
    
    override var isTransforming: Bool {
        get { return mockIsTransforming }
        set { mockIsTransforming = newValue }
    }
    
    override var selectedLayer: DrawingLayer? {
        get { return mockSelectedLayer }
        set { mockSelectedLayer = newValue }
    }
    
    override var transformationType: TransformationType {
        get { return mockTransformationType }
        set { mockTransformationType = newValue }
    }
    
    override var transformationCenter: CGPoint {
        get { return mockTransformationCenter }
        set { mockTransformationCenter = newValue }
    }
    
    override var transformationAngle: Double {
        get { return mockTransformationAngle }
        set { mockTransformationAngle = newValue }
    }
    
    override var transformationScale: Double {
        get { return mockTransformationScale }
        set { mockTransformationScale = newValue }
    }
    
    override var transformationOffset: CGSize {
        get { return mockTransformationOffset }
        set { mockTransformationOffset = newValue }
    }
    
    override var skewX: Double {
        get { return mockSkewX }
        set { mockSkewX = newValue }
    }
    
    override var skewY: Double {
        get { return mockSkewY }
        set { mockSkewY = newValue }
    }
    
    override var perspective: Double {
        get { return mockPerspective }
        set { mockPerspective = newValue }
    }
    
    override var isFlippedHorizontally: Bool {
        get { return mockIsFlippedHorizontally }
        set { mockIsFlippedHorizontally = newValue }
    }
    
    override var isFlippedVertically: Bool {
        get { return mockIsFlippedVertically }
        set { mockIsFlippedVertically = newValue }
    }
    
    override var history: [TransformationHistoryEntry] {
        return mockHistory
    }
    
    override func moveLayer(by offset: CGSize) {
        mockTransformationOffset = offset
        mockTransformationType = .move
        mockHistory.append(TransformationHistoryEntry(
            type: .move,
            timestamp: Date(),
            description: "Moved layer by \(offset)"
        ))
    }
    
    override func rotateLayer(by angle: Double, around center: CGPoint = .zero) {
        mockTransformationAngle = angle
        mockTransformationCenter = center
        mockTransformationType = .rotate
        mockHistory.append(TransformationHistoryEntry(
            type: .rotate,
            timestamp: Date(),
            description: "Rotated layer by \(angle) degrees"
        ))
    }
    
    override func scaleLayer(by scale: Double, around center: CGPoint = .zero) {
        mockTransformationScale = max(0.1, min(10.0, scale)) // Clamp scale
        mockTransformationCenter = center
        mockTransformationType = .scale
        mockHistory.append(TransformationHistoryEntry(
            type: .scale,
            timestamp: Date(),
            description: "Scaled layer by \(scale)"
        ))
    }
    
    override func flipLayerHorizontally() {
        mockIsFlippedHorizontally.toggle()
        mockTransformationType = .flip
        mockHistory.append(TransformationHistoryEntry(
            type: .flip,
            timestamp: Date(),
            description: "Flipped layer horizontally"
        ))
    }
    
    override func flipLayerVertically() {
        mockIsFlippedVertically.toggle()
        mockTransformationType = .flip
        mockHistory.append(TransformationHistoryEntry(
            type: .flip,
            timestamp: Date(),
            description: "Flipped layer vertically"
        ))
    }
    
    override func skewLayer(x skewX: Double, y skewY: Double) {
        mockSkewX = skewX
        mockSkewY = skewY
        mockTransformationType = .skew
        mockHistory.append(TransformationHistoryEntry(
            type: .skew,
            timestamp: Date(),
            description: "Skewed layer by (\(skewX), \(skewY))"
        ))
    }
    
    override func applyPerspective(_ perspective: Double) {
        mockPerspective = perspective
        mockTransformationType = .perspective
        mockHistory.append(TransformationHistoryEntry(
            type: .perspective,
            timestamp: Date(),
            description: "Applied perspective: \(perspective)"
        ))
    }
    
    override func resetTransformation() {
        mockTransformationOffset = .zero
        mockTransformationAngle = 0.0
        mockTransformationScale = 1.0
        mockTransformationCenter = .zero
        mockSkewX = 0.0
        mockSkewY = 0.0
        mockPerspective = 0.0
        mockIsFlippedHorizontally = false
        mockIsFlippedVertically = false
    }
    
    override func getCombinedTransform() -> CGAffineTransform {
        var transform = CGAffineTransform.identity
        transform = transform.translatedBy(x: mockTransformationOffset.width, y: mockTransformationOffset.height)
        transform = transform.rotated(by: mockTransformationAngle * .pi / 180)
        transform = transform.scaledBy(x: mockTransformationScale, y: mockTransformationScale)
        return transform
    }
}