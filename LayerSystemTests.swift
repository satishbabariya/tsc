import XCTest
import SwiftUI
@testable import ColorFillingApp

class LayerSystemTests: XCTestCase {
    var layerManager: LayerManager!
    
    override func setUp() {
        super.setUp()
        layerManager = LayerManager()
    }
    
    override func tearDown() {
        layerManager = nil
        super.tearDown()
    }
    
    // MARK: - Layer Creation Tests
    
    func testLayerCreation() {
        let layer = layerManager.createLayer(name: "Test Layer")
        
        XCTAssertEqual(layer.name, "Test Layer", "Layer should have correct name")
        XCTAssertEqual(layer.opacity, 1.0, "Layer should have default opacity")
        XCTAssertTrue(layer.isVisible, "Layer should be visible by default")
        XCTAssertEqual(layer.blendingMode, .normal, "Layer should have normal blending mode")
        XCTAssertNotNil(layer.id, "Layer should have unique ID")
        XCTAssertNotNil(layer.timestamp, "Layer should have timestamp")
    }
    
    func testLayerCreationWithCustomProperties() {
        let layer = layerManager.createLayer(
            name: "Custom Layer",
            opacity: 0.7,
            blendingMode: .multiply,
            isVisible: false
        )
        
        XCTAssertEqual(layer.name, "Custom Layer", "Layer should have correct name")
        XCTAssertEqual(layer.opacity, 0.7, "Layer should have custom opacity")
        XCTAssertFalse(layer.isVisible, "Layer should not be visible")
        XCTAssertEqual(layer.blendingMode, .multiply, "Layer should have multiply blending mode")
    }
    
    func testLayerCreationWithEmptyName() {
        let layer = layerManager.createLayer(name: "")
        
        XCTAssertEqual(layer.name, "Untitled Layer", "Empty name should default to 'Untitled Layer'")
    }
    
    func testLayerCreationWithNilName() {
        let layer = layerManager.createLayer(name: nil)
        
        XCTAssertEqual(layer.name, "Untitled Layer", "Nil name should default to 'Untitled Layer'")
    }
    
    // MARK: - Layer Management Tests
    
    func testAddLayer() {
        let initialCount = layerManager.layers.count
        let layer = layerManager.createLayer(name: "New Layer")
        layerManager.addLayer(layer)
        
        XCTAssertEqual(layerManager.layers.count, initialCount + 1, "Layer count should increase")
        XCTAssertTrue(layerManager.layers.contains { $0.id == layer.id }, "Layer should be in layers array")
    }
    
    func testRemoveLayer() {
        let layer = layerManager.createLayer(name: "To Remove")
        layerManager.addLayer(layer)
        
        let initialCount = layerManager.layers.count
        layerManager.removeLayer(layer)
        
        XCTAssertEqual(layerManager.layers.count, initialCount - 1, "Layer count should decrease")
        XCTAssertFalse(layerManager.layers.contains { $0.id == layer.id }, "Layer should not be in layers array")
    }
    
    func testRemoveNonExistentLayer() {
        let layer = layerManager.createLayer(name: "Non Existent")
        let initialCount = layerManager.layers.count
        
        layerManager.removeLayer(layer)
        
        XCTAssertEqual(layerManager.layers.count, initialCount, "Layer count should not change")
    }
    
    func testDuplicateLayer() {
        let originalLayer = layerManager.createLayer(name: "Original")
        layerManager.addLayer(originalLayer)
        
        let duplicatedLayer = layerManager.duplicateLayer(originalLayer)
        
        XCTAssertEqual(duplicatedLayer.name, "Original Copy", "Duplicated layer should have 'Copy' suffix")
        XCTAssertEqual(duplicatedLayer.opacity, originalLayer.opacity, "Duplicated layer should have same opacity")
        XCTAssertEqual(duplicatedLayer.isVisible, originalLayer.isVisible, "Duplicated layer should have same visibility")
        XCTAssertEqual(duplicatedLayer.blendingMode, originalLayer.blendingMode, "Duplicated layer should have same blending mode")
        XCTAssertNotEqual(duplicatedLayer.id, originalLayer.id, "Duplicated layer should have different ID")
    }
    
    func testMergeLayers() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        
        let initialCount = layerManager.layers.count
        let mergedLayer = layerManager.mergeLayers([layer1, layer2])
        
        XCTAssertEqual(layerManager.layers.count, initialCount - 1, "Layer count should decrease by 1")
        XCTAssertEqual(mergedLayer.name, "Merged Layer", "Merged layer should have correct name")
        XCTAssertFalse(layerManager.layers.contains { $0.id == layer1.id }, "Original layer 1 should be removed")
        XCTAssertFalse(layerManager.layers.contains { $0.id == layer2.id }, "Original layer 2 should be removed")
    }
    
    func testMergeSingleLayer() {
        let layer = layerManager.createLayer(name: "Single Layer")
        layerManager.addLayer(layer)
        
        let initialCount = layerManager.layers.count
        let mergedLayer = layerManager.mergeLayers([layer])
        
        XCTAssertEqual(layerManager.layers.count, initialCount, "Layer count should not change")
        XCTAssertEqual(mergedLayer.id, layer.id, "Single layer merge should return same layer")
    }
    
    func testMergeEmptyLayers() {
        let initialCount = layerManager.layers.count
        let mergedLayer = layerManager.mergeLayers([])
        
        XCTAssertEqual(layerManager.layers.count, initialCount, "Layer count should not change")
        XCTAssertNil(mergedLayer, "Merging empty layers should return nil")
    }
    
    // MARK: - Layer Ordering Tests
    
    func testMoveLayerUp() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        let layer3 = layerManager.createLayer(name: "Layer 3")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        layerManager.addLayer(layer3)
        
        layerManager.moveLayerUp(layer2)
        
        XCTAssertEqual(layerManager.layers[0].id, layer1.id, "Layer 1 should remain first")
        XCTAssertEqual(layerManager.layers[1].id, layer3.id, "Layer 3 should be second")
        XCTAssertEqual(layerManager.layers[2].id, layer2.id, "Layer 2 should be third")
    }
    
    func testMoveLayerDown() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        let layer3 = layerManager.createLayer(name: "Layer 3")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        layerManager.addLayer(layer3)
        
        layerManager.moveLayerDown(layer2)
        
        XCTAssertEqual(layerManager.layers[0].id, layer2.id, "Layer 2 should be first")
        XCTAssertEqual(layerManager.layers[1].id, layer1.id, "Layer 1 should be second")
        XCTAssertEqual(layerManager.layers[2].id, layer3.id, "Layer 3 should remain third")
    }
    
    func testMoveLayerToTop() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        let layer3 = layerManager.createLayer(name: "Layer 3")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        layerManager.addLayer(layer3)
        
        layerManager.moveLayerToTop(layer2)
        
        XCTAssertEqual(layerManager.layers[0].id, layer2.id, "Layer 2 should be first")
        XCTAssertEqual(layerManager.layers[1].id, layer1.id, "Layer 1 should be second")
        XCTAssertEqual(layerManager.layers[2].id, layer3.id, "Layer 3 should be third")
    }
    
    func testMoveLayerToBottom() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        let layer3 = layerManager.createLayer(name: "Layer 3")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        layerManager.addLayer(layer3)
        
        layerManager.moveLayerToBottom(layer2)
        
        XCTAssertEqual(layerManager.layers[0].id, layer1.id, "Layer 1 should remain first")
        XCTAssertEqual(layerManager.layers[1].id, layer3.id, "Layer 3 should be second")
        XCTAssertEqual(layerManager.layers[2].id, layer2.id, "Layer 2 should be third")
    }
    
    func testMoveLayerToIndex() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        let layer3 = layerManager.createLayer(name: "Layer 3")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        layerManager.addLayer(layer3)
        
        layerManager.moveLayer(layer2, to: 0)
        
        XCTAssertEqual(layerManager.layers[0].id, layer2.id, "Layer 2 should be first")
        XCTAssertEqual(layerManager.layers[1].id, layer1.id, "Layer 1 should be second")
        XCTAssertEqual(layerManager.layers[2].id, layer3.id, "Layer 3 should be third")
    }
    
    // MARK: - Layer Properties Tests
    
    func testSetLayerOpacity() {
        let layer = layerManager.createLayer(name: "Test Layer")
        layerManager.addLayer(layer)
        
        layerManager.setLayerOpacity(layer, opacity: 0.5)
        
        let updatedLayer = layerManager.layers.first { $0.id == layer.id }!
        XCTAssertEqual(updatedLayer.opacity, 0.5, "Layer opacity should be updated")
    }
    
    func testSetLayerOpacityBounds() {
        let layer = layerManager.createLayer(name: "Test Layer")
        layerManager.addLayer(layer)
        
        // Test negative opacity
        layerManager.setLayerOpacity(layer, opacity: -0.5)
        let updatedLayer1 = layerManager.layers.first { $0.id == layer.id }!
        XCTAssertEqual(updatedLayer1.opacity, 0.0, "Negative opacity should be clamped to 0.0")
        
        // Test opacity greater than 1.0
        layerManager.setLayerOpacity(layer, opacity: 1.5)
        let updatedLayer2 = layerManager.layers.first { $0.id == layer.id }!
        XCTAssertEqual(updatedLayer2.opacity, 1.0, "Opacity greater than 1.0 should be clamped to 1.0")
    }
    
    func testSetLayerVisibility() {
        let layer = layerManager.createLayer(name: "Test Layer")
        layerManager.addLayer(layer)
        
        layerManager.setLayerVisibility(layer, isVisible: false)
        
        let updatedLayer = layerManager.layers.first { $0.id == layer.id }!
        XCTAssertFalse(updatedLayer.isVisible, "Layer visibility should be updated")
    }
    
    func testSetLayerBlendingMode() {
        let layer = layerManager.createLayer(name: "Test Layer")
        layerManager.addLayer(layer)
        
        layerManager.setLayerBlendingMode(layer, mode: .multiply)
        
        let updatedLayer = layerManager.layers.first { $0.id == layer.id }!
        XCTAssertEqual(updatedLayer.blendingMode, .multiply, "Layer blending mode should be updated")
    }
    
    func testSetLayerName() {
        let layer = layerManager.createLayer(name: "Old Name")
        layerManager.addLayer(layer)
        
        layerManager.setLayerName(layer, name: "New Name")
        
        let updatedLayer = layerManager.layers.first { $0.id == layer.id }!
        XCTAssertEqual(updatedLayer.name, "New Name", "Layer name should be updated")
    }
    
    func testSetLayerNameWithEmptyString() {
        let layer = layerManager.createLayer(name: "Original Name")
        layerManager.addLayer(layer)
        
        layerManager.setLayerName(layer, name: "")
        
        let updatedLayer = layerManager.layers.first { $0.id == layer.id }!
        XCTAssertEqual(updatedLayer.name, "Untitled Layer", "Empty name should default to 'Untitled Layer'")
    }
    
    // MARK: - Blending Mode Tests
    
    func testAllBlendingModes() {
        let blendingModes: [BlendingMode] = [
            .normal, .multiply, .screen, .overlay, .softLight, .hardLight,
            .colorDodge, .colorBurn, .darken, .lighten, .difference,
            .exclusion, .hue, .saturation, .color, .luminosity
        ]
        
        for mode in blendingModes {
            let layer = layerManager.createLayer(name: "Test Layer", blendingMode: mode)
            XCTAssertEqual(layer.blendingMode, mode, "Layer should have correct blending mode: \(mode)")
        }
    }
    
    func testBlendingModeApplication() {
        let layer1 = layerManager.createLayer(name: "Layer 1", blendingMode: .normal)
        let layer2 = layerManager.createLayer(name: "Layer 2", blendingMode: .multiply)
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        
        XCTAssertEqual(layerManager.layers[0].blendingMode, .normal, "First layer should have normal blending mode")
        XCTAssertEqual(layerManager.layers[1].blendingMode, .multiply, "Second layer should have multiply blending mode")
    }
    
    // MARK: - Layer Grouping Tests
    
    func testCreateLayerGroup() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        
        let group = layerManager.createLayerGroup(name: "Test Group", layers: [layer1, layer2])
        
        XCTAssertEqual(group.name, "Test Group", "Group should have correct name")
        XCTAssertEqual(group.layers.count, 2, "Group should contain 2 layers")
        XCTAssertTrue(group.layers.contains { $0.id == layer1.id }, "Group should contain layer 1")
        XCTAssertTrue(group.layers.contains { $0.id == layer2.id }, "Group should contain layer 2")
    }
    
    func testUngroupLayers() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        
        let group = layerManager.createLayerGroup(name: "Test Group", layers: [layer1, layer2])
        layerManager.addLayerGroup(group)
        
        let initialCount = layerManager.layers.count
        layerManager.ungroupLayers(group)
        
        XCTAssertEqual(layerManager.layers.count, initialCount + 1, "Layer count should increase by 1")
        XCTAssertTrue(layerManager.layers.contains { $0.id == layer1.id }, "Layer 1 should be ungrouped")
        XCTAssertTrue(layerManager.layers.contains { $0.id == layer2.id }, "Layer 2 should be ungrouped")
    }
    
    // MARK: - Layer Flattening Tests
    
    func testFlattenAllLayers() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        let layer2 = layerManager.createLayer(name: "Layer 2")
        let layer3 = layerManager.createLayer(name: "Layer 3")
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        layerManager.addLayer(layer3)
        
        let flattenedLayer = layerManager.flattenAllLayers()
        
        XCTAssertEqual(layerManager.layers.count, 1, "Should have only one flattened layer")
        XCTAssertEqual(flattenedLayer.name, "Flattened Layer", "Flattened layer should have correct name")
        XCTAssertEqual(flattenedLayer.opacity, 1.0, "Flattened layer should have full opacity")
        XCTAssertTrue(flattenedLayer.isVisible, "Flattened layer should be visible")
    }
    
    func testFlattenVisibleLayers() {
        let layer1 = layerManager.createLayer(name: "Layer 1", isVisible: true)
        let layer2 = layerManager.createLayer(name: "Layer 2", isVisible: false)
        let layer3 = layerManager.createLayer(name: "Layer 3", isVisible: true)
        
        layerManager.addLayer(layer1)
        layerManager.addLayer(layer2)
        layerManager.addLayer(layer3)
        
        let flattenedLayer = layerManager.flattenVisibleLayers()
        
        XCTAssertEqual(layerManager.layers.count, 2, "Should have flattened layer and hidden layer")
        XCTAssertTrue(layerManager.layers.contains { $0.id == layer2.id }, "Hidden layer should remain")
        XCTAssertEqual(flattenedLayer.name, "Flattened Visible Layers", "Flattened layer should have correct name")
    }
    
    // MARK: - Layer History Tests
    
    func testLayerHistory() {
        let layer1 = layerManager.createLayer(name: "Layer 1")
        layerManager.addLayer(layer1)
        
        let layer2 = layerManager.createLayer(name: "Layer 2")
        layerManager.addLayer(layer2)
        
        XCTAssertEqual(layerManager.history.count, 2, "History should track layer additions")
        XCTAssertEqual(layerManager.history[0].action, .layerAdded, "First history entry should be layer added")
        XCTAssertEqual(layerManager.history[1].action, .layerAdded, "Second history entry should be layer added")
    }
    
    func testUndoLayerOperation() {
        let layer = layerManager.createLayer(name: "Test Layer")
        layerManager.addLayer(layer)
        
        let initialCount = layerManager.layers.count
        layerManager.undo()
        
        XCTAssertEqual(layerManager.layers.count, initialCount - 1, "Undo should remove layer")
        XCTAssertFalse(layerManager.layers.contains { $0.id == layer.id }, "Layer should not be in layers array")
    }
    
    func testRedoLayerOperation() {
        let layer = layerManager.createLayer(name: "Test Layer")
        layerManager.addLayer(layer)
        
        layerManager.undo()
        let initialCount = layerManager.layers.count
        
        layerManager.redo()
        
        XCTAssertEqual(layerManager.layers.count, initialCount + 1, "Redo should restore layer")
        XCTAssertTrue(layerManager.layers.contains { $0.id == layer.id }, "Layer should be restored")
    }
    
    // MARK: - Edge Cases Tests
    
    func testLayerWithSpecialCharacters() {
        let layer = layerManager.createLayer(name: "Layer with 🎨 emoji and special chars!@#$%")
        
        XCTAssertEqual(layer.name, "Layer with 🎨 emoji and special chars!@#$%", "Layer should handle special characters")
    }
    
    func testLayerWithVeryLongName() {
        let longName = String(repeating: "A", count: 1000)
        let layer = layerManager.createLayer(name: longName)
        
        XCTAssertEqual(layer.name, longName, "Layer should handle very long names")
    }
    
    func testLayerWithNilProperties() {
        let layer = layerManager.createLayer(name: nil, opacity: nil, blendingMode: nil, isVisible: nil)
        
        XCTAssertEqual(layer.name, "Untitled Layer", "Nil name should default to 'Untitled Layer'")
        XCTAssertEqual(layer.opacity, 1.0, "Nil opacity should default to 1.0")
        XCTAssertEqual(layer.blendingMode, .normal, "Nil blending mode should default to normal")
        XCTAssertTrue(layer.isVisible, "Nil visibility should default to true")
    }
    
    // MARK: - Performance Tests
    
    func testPerformanceLargeNumberOfLayers() {
        measure {
            for i in 0..<1000 {
                let layer = layerManager.createLayer(name: "Layer \(i)")
                layerManager.addLayer(layer)
            }
        }
    }
    
    func testPerformanceLayerOperations() {
        // Create many layers first
        for i in 0..<100 {
            let layer = layerManager.createLayer(name: "Layer \(i)")
            layerManager.addLayer(layer)
        }
        
        measure {
            // Perform various operations
            for i in 0..<50 {
                let layer = layerManager.layers[i]
                layerManager.setLayerOpacity(layer, opacity: Double(i) / 100.0)
                layerManager.setLayerVisibility(layer, isVisible: i % 2 == 0)
            }
        }
    }
    
    func testPerformanceLayerMerging() {
        // Create many layers first
        for i in 0..<100 {
            let layer = layerManager.createLayer(name: "Layer \(i)")
            layerManager.addLayer(layer)
        }
        
        measure {
            // Merge layers in groups of 5
            for i in stride(from: 0, to: 100, by: 5) {
                let layersToMerge = Array(layerManager.layers[i..<min(i+5, 100)])
                _ = layerManager.mergeLayers(layersToMerge)
            }
        }
    }
}

// MARK: - Mock Objects for Testing

class MockLayerManager: LayerManager {
    var mockLayers: [DrawingLayer] = []
    var mockHistory: [LayerHistoryEntry] = []
    
    override var layers: [DrawingLayer] {
        return mockLayers
    }
    
    override var history: [LayerHistoryEntry] {
        return mockHistory
    }
    
    override func addLayer(_ layer: DrawingLayer) {
        mockLayers.append(layer)
        mockHistory.append(LayerHistoryEntry(
            action: .layerAdded,
            layerId: layer.id,
            timestamp: Date(),
            description: "Added layer: \(layer.name)"
        ))
    }
    
    override func removeLayer(_ layer: DrawingLayer) {
        mockLayers.removeAll { $0.id == layer.id }
        mockHistory.append(LayerHistoryEntry(
            action: .layerRemoved,
            layerId: layer.id,
            timestamp: Date(),
            description: "Removed layer: \(layer.name)"
        ))
    }
    
    override func duplicateLayer(_ layer: DrawingLayer) -> DrawingLayer {
        let duplicatedLayer = DrawingLayer(
            id: UUID(),
            name: "\(layer.name) Copy",
            opacity: layer.opacity,
            blendingMode: layer.blendingMode,
            isVisible: layer.isVisible,
            timestamp: Date()
        )
        mockLayers.append(duplicatedLayer)
        return duplicatedLayer
    }
}