import XCTest
import SwiftUI
@testable import ColorFillingApp

class ProfessionalIntegrationTests: XCTestCase {
    
    var professionalApp: ProfessionalColorFillingApp!
    var contentView: ProfessionalContentView!
    
    // Managers
    var drawingManager: DrawingManager!
    var layerManager: LayerManager!
    var selectionManager: SelectionManager!
    var transformationManager: TransformationManager!
    var filterManager: FilterManager!
    var textToolManager: TextToolManager!
    var gradientPatternManager: GradientPatternManager!
    var advancedColorManager: AdvancedColorManager!
    var canvasManager: CanvasManager!
    var exportManager: ExportManager!
    
    override func setUp() {
        super.setUp()
        setupProfessionalApp()
    }
    
    override func tearDown() {
        professionalApp = nil
        contentView = nil
        drawingManager = nil
        layerManager = nil
        selectionManager = nil
        transformationManager = nil
        filterManager = nil
        textToolManager = nil
        gradientPatternManager = nil
        advancedColorManager = nil
        canvasManager = nil
        exportManager = nil
        super.tearDown()
    }
    
    private func setupProfessionalApp() {
        // Initialize all managers
        drawingManager = DrawingManager()
        layerManager = LayerManager()
        selectionManager = SelectionManager()
        transformationManager = TransformationManager()
        filterManager = FilterManager()
        textToolManager = TextToolManager()
        gradientPatternManager = GradientPatternManager()
        advancedColorManager = AdvancedColorManager()
        canvasManager = CanvasManager()
        exportManager = ExportManager()
        
        // Initialize professional app
        professionalApp = ProfessionalColorFillingApp()
        contentView = ProfessionalContentView()
    }
    
    // MARK: - Professional Mode Integration Tests
    
    func testProfessionalModeSwitching() {
        // Test switching between professional modes
        let modes: [ProfessionalContentView.ProfessionalMode] = [.drawing, .coloring, .text, .design]
        
        for mode in modes {
            contentView.currentMode = mode
            
            switch mode {
            case .drawing:
                XCTAssertEqual(contentView.currentMode, .drawing, "Should be in drawing mode")
                XCTAssertNotNil(drawingManager, "Drawing manager should be available")
                XCTAssertNotNil(layerManager, "Layer manager should be available")
                
            case .coloring:
                XCTAssertEqual(contentView.currentMode, .coloring, "Should be in coloring mode")
                XCTAssertNotNil(contentView.selectedColor, "Selected color should be available")
                
            case .text:
                XCTAssertEqual(contentView.currentMode, .text, "Should be in text mode")
                XCTAssertNotNil(textToolManager, "Text tool manager should be available")
                
            case .design:
                XCTAssertEqual(contentView.currentMode, .design, "Should be in design mode")
                XCTAssertNotNil(gradientPatternManager, "Gradient pattern manager should be available")
            }
        }
    }
    
    func testProfessionalModeToolIntegration() {
        // Test that tools are properly integrated with their respective modes
        
        // Drawing mode tools
        contentView.currentMode = .drawing
        XCTAssertNotNil(drawingManager.selectedTool, "Drawing tool should be available")
        XCTAssertNotNil(drawingManager.brushSettings, "Brush settings should be available")
        
        // Text mode tools
        contentView.currentMode = .text
        XCTAssertNotNil(textToolManager.selectedFont, "Text font should be available")
        XCTAssertNotNil(textToolManager.textElements, "Text elements should be available")
        
        // Design mode tools
        contentView.currentMode = .design
        XCTAssertNotNil(gradientPatternManager.gradients, "Gradients should be available")
        XCTAssertNotNil(gradientPatternManager.patterns, "Patterns should be available")
    }
    
    // MARK: - Cross-Module Integration Tests
    
    func testDrawingAndLayerIntegration() {
        // Test that drawing operations work with layer system
        let layer = layerManager.createLayer(name: "Drawing Layer")
        layerManager.addLayer(layer)
        
        drawingManager.selectedLayer = layer
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.addPoint(CGPoint(x: 20, y: 20))
        drawingManager.endStroke()
        
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should have one stroke")
        XCTAssertEqual(layerManager.layers.count, 1, "Should have one layer")
        XCTAssertEqual(drawingManager.selectedLayer?.id, layer.id, "Drawing should be associated with layer")
    }
    
    func testSelectionAndTransformationIntegration() {
        // Test that selection and transformation work together
        let layer = layerManager.createLayer(name: "Test Layer")
        layerManager.addLayer(layer)
        
        // Create selection
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        // Transform selection
        transformationManager.selectedLayer = layer
        transformationManager.moveLayer(by: CGSize(width: 10, height: 10))
        
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        XCTAssertEqual(transformationManager.transformationOffset, CGSize(width: 10, height: 10), "Transformation should be applied")
    }
    
    func testLayerAndFilterIntegration() {
        // Test that filters work with layers
        let layer = layerManager.createLayer(name: "Filter Layer")
        layerManager.addLayer(layer)
        
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 1, "Should have one applied filter")
        XCTAssertEqual(filterManager.appliedFilters.first?.layerId, layer.id, "Filter should be applied to correct layer")
    }
    
    func testCanvasAndAllToolsIntegration() {
        // Test that canvas works with all tools
        let canvas = canvasManager.canvas
        
        // Test canvas with drawing
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.addPoint(CGPoint(x: 20, y: 20))
        drawingManager.endStroke()
        
        // Test canvas with layers
        let layer = layerManager.createLayer(name: "Canvas Layer")
        layerManager.addLayer(layer)
        
        // Test canvas with selections
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 10, y: 10))
        selectionManager.updateSelection(to: CGPoint(x: 50, y: 50))
        selectionManager.endSelection()
        
        // Test canvas with transformations
        transformationManager.selectedLayer = layer
        transformationManager.moveLayer(by: CGSize(width: 10, height: 10))
        
        XCTAssertNotNil(canvas, "Canvas should be available")
        XCTAssertEqual(canvas.size, CGSize(width: 800, height: 600), "Canvas should have correct size")
    }
    
    func testColorManagementIntegration() {
        // Test that color management works across all tools
        let testColor = Color.red
        
        // Test with drawing manager
        drawingManager.selectedColor = testColor
        XCTAssertEqual(drawingManager.selectedColor, testColor, "Drawing manager should use selected color")
        
        // Test with advanced color manager
        advancedColorManager.selectedColor = testColor
        XCTAssertEqual(advancedColorManager.selectedColor, testColor, "Advanced color manager should use selected color")
        
        // Test color history
        XCTAssertEqual(drawingManager.colorHistory.count, 1, "Color should be added to history")
        XCTAssertEqual(advancedColorManager.colorHistory.count, 1, "Color should be added to advanced history")
    }
    
    // MARK: - Professional Workflow Integration Tests
    
    func testCompleteDrawingWorkflow() {
        // Test complete professional drawing workflow
        
        // 1. Create layer
        let layer = layerManager.createLayer(name: "Professional Drawing")
        layerManager.addLayer(layer)
        
        // 2. Set up drawing tools
        drawingManager.selectedTool = .brush
        drawingManager.brushSettings.size = 15.0
        drawingManager.brushSettings.opacity = 0.8
        drawingManager.selectedColor = Color.blue
        
        // 3. Draw strokes
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.addPoint(CGPoint(x: 30, y: 30))
        drawingManager.addPoint(CGPoint(x: 50, y: 10))
        drawingManager.endStroke()
        
        // 4. Create selection
        selectionManager.selectedTool = .rectangle
        selectionManager.startSelection(at: CGPoint(x: 5, y: 5))
        selectionManager.updateSelection(to: CGPoint(x: 55, y: 35))
        selectionManager.endSelection()
        
        // 5. Apply transformation
        transformationManager.selectedLayer = layer
        transformationManager.moveLayer(by: CGSize(width: 10, height: 10))
        transformationManager.rotateLayer(by: 45.0)
        
        // 6. Apply filter
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.3
        filterManager.applyFilter(to: layer)
        
        // 7. Export
        exportManager.selectedFormat = .png
        exportManager.quality = .high
        
        // Verify workflow
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should have one stroke")
        XCTAssertEqual(layerManager.layers.count, 1, "Should have one layer")
        XCTAssertEqual(selectionManager.selections.count, 1, "Should have one selection")
        XCTAssertEqual(transformationManager.transformationOffset, CGSize(width: 10, height: 10), "Should have transformation")
        XCTAssertEqual(filterManager.appliedFilters.count, 1, "Should have one applied filter")
        XCTAssertEqual(exportManager.selectedFormat, .png, "Should have export format")
    }
    
    func testCompleteTextWorkflow() {
        // Test complete professional text workflow
        
        // 1. Create text element
        textToolManager.selectedFont = .system(size: 24, weight: .bold)
        textToolManager.selectedColor = Color.black
        textToolManager.addTextElement(at: CGPoint(x: 50, y: 50))
        
        // 2. Format text
        if let textElement = textToolManager.textElements.first {
            textToolManager.updateTextElement(textElement, text: "Professional Text")
            textToolManager.setTextAlignment(textElement, alignment: .center)
            textToolManager.setTextOpacity(textElement, opacity: 0.9)
        }
        
        // 3. Create layer for text
        let layer = layerManager.createLayer(name: "Text Layer")
        layerManager.addLayer(layer)
        
        // 4. Apply transformation to text
        transformationManager.selectedLayer = layer
        transformationManager.moveLayer(by: CGSize(width: 20, height: 20))
        transformationManager.scaleLayer(by: 1.2)
        
        // 5. Apply filter to text
        filterManager.selectedFilter = .emboss
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        // Verify workflow
        XCTAssertEqual(textToolManager.textElements.count, 1, "Should have one text element")
        XCTAssertEqual(layerManager.layers.count, 1, "Should have one layer")
        XCTAssertEqual(transformationManager.transformationOffset, CGSize(width: 20, height: 20), "Should have transformation")
        XCTAssertEqual(filterManager.appliedFilters.count, 1, "Should have one applied filter")
    }
    
    func testCompleteDesignWorkflow() {
        // Test complete professional design workflow
        
        // 1. Create gradient
        gradientPatternManager.selectedGradientType = .linear
        gradientPatternManager.createGradient(
            startPoint: CGPoint(x: 0, y: 0),
            endPoint: CGPoint(x: 100, y: 100),
            colors: [Color.red, Color.blue]
        )
        
        // 2. Create pattern
        gradientPatternManager.selectedPatternType = .dots
        gradientPatternManager.createPattern(
            center: CGPoint(x: 50, y: 50),
            size: CGSize(width: 100, height: 100),
            color: Color.green
        )
        
        // 3. Create layer for design
        let layer = layerManager.createLayer(name: "Design Layer")
        layerManager.addLayer(layer)
        
        // 4. Apply transformation to design
        transformationManager.selectedLayer = layer
        transformationManager.moveLayer(by: CGSize(width: 30, height: 30))
        transformationManager.rotateLayer(by: 30.0)
        
        // 5. Apply filter to design
        filterManager.selectedFilter = .vibrance
        filterManager.filterIntensity = 0.7
        filterManager.applyFilter(to: layer)
        
        // Verify workflow
        XCTAssertEqual(gradientPatternManager.gradients.count, 1, "Should have one gradient")
        XCTAssertEqual(gradientPatternManager.patterns.count, 1, "Should have one pattern")
        XCTAssertEqual(layerManager.layers.count, 1, "Should have one layer")
        XCTAssertEqual(transformationManager.transformationOffset, CGSize(width: 30, height: 30), "Should have transformation")
        XCTAssertEqual(filterManager.appliedFilters.count, 1, "Should have one applied filter")
    }
    
    // MARK: - Professional Mode State Management Tests
    
    func testProfessionalModeStatePersistence() {
        // Test that professional mode state is properly maintained
        
        // Set up drawing mode
        contentView.currentMode = .drawing
        drawingManager.selectedTool = .brush
        drawingManager.brushSettings.size = 20.0
        drawingManager.selectedColor = Color.red
        
        // Switch to text mode
        contentView.currentMode = .text
        textToolManager.selectedFont = .system(size: 18, weight: .medium)
        textToolManager.selectedColor = Color.blue
        
        // Switch back to drawing mode
        contentView.currentMode = .drawing
        
        // Verify state persistence
        XCTAssertEqual(contentView.currentMode, .drawing, "Should be back in drawing mode")
        XCTAssertEqual(drawingManager.selectedTool, .brush, "Drawing tool should be preserved")
        XCTAssertEqual(drawingManager.brushSettings.size, 20.0, "Brush size should be preserved")
        XCTAssertEqual(drawingManager.selectedColor, Color.red, "Drawing color should be preserved")
    }
    
    func testProfessionalModeDataIsolation() {
        // Test that data is properly isolated between modes
        
        // Set up drawing mode data
        contentView.currentMode = .drawing
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.addPoint(CGPoint(x: 20, y: 20))
        drawingManager.endStroke()
        
        // Set up text mode data
        contentView.currentMode = .text
        textToolManager.addTextElement(at: CGPoint(x: 30, y: 30))
        
        // Set up design mode data
        contentView.currentMode = .design
        gradientPatternManager.createGradient(
            startPoint: CGPoint(x: 0, y: 0),
            endPoint: CGPoint(x: 50, y: 50),
            colors: [Color.green, Color.yellow]
        )
        
        // Verify data isolation
        XCTAssertEqual(drawingManager.strokes.count, 1, "Drawing data should be isolated")
        XCTAssertEqual(textToolManager.textElements.count, 1, "Text data should be isolated")
        XCTAssertEqual(gradientPatternManager.gradients.count, 1, "Design data should be isolated")
    }
    
    // MARK: - Professional Performance Integration Tests
    
    func testProfessionalModePerformance() {
        // Test performance of professional mode operations
        
        measure {
            // Simulate professional workflow
            for _ in 0..<100 {
                // Create layer
                let layer = layerManager.createLayer(name: "Performance Layer")
                layerManager.addLayer(layer)
                
                // Draw stroke
                drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
                drawingManager.addPoint(CGPoint(x: 20, y: 20))
                drawingManager.endStroke()
                
                // Create selection
                selectionManager.startSelection(at: CGPoint(x: 5, y: 5))
                selectionManager.updateSelection(to: CGPoint(x: 25, y: 25))
                selectionManager.endSelection()
                
                // Apply transformation
                transformationManager.selectedLayer = layer
                transformationManager.moveLayer(by: CGSize(width: 5, y: 5))
                
                // Apply filter
                filterManager.selectedFilter = .brightness
                filterManager.filterIntensity = 0.5
                filterManager.applyFilter(to: layer)
            }
        }
    }
    
    func testProfessionalModeMemoryManagement() {
        // Test memory management in professional mode
        
        // Create many objects
        for i in 0..<1000 {
            let layer = layerManager.createLayer(name: "Memory Layer \(i)")
            layerManager.addLayer(layer)
            
            drawingManager.startStroke(at: CGPoint(x: i, y: i))
            drawingManager.addPoint(CGPoint(x: i + 10, y: i + 10))
            drawingManager.endStroke()
            
            filterManager.selectedFilter = .brightness
            filterManager.filterIntensity = 0.5
            filterManager.applyFilter(to: layer)
        }
        
        // Clear all data
        layerManager.clearAllLayers()
        drawingManager.clearAllStrokes()
        filterManager.clearAllFilters()
        
        // Verify memory cleanup
        XCTAssertEqual(layerManager.layers.count, 0, "Layers should be cleared")
        XCTAssertEqual(drawingManager.strokes.count, 0, "Strokes should be cleared")
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Filters should be cleared")
    }
    
    // MARK: - Professional Error Handling Integration Tests
    
    func testProfessionalModeErrorHandling() {
        // Test error handling across professional modes
        
        // Test drawing mode error handling
        contentView.currentMode = .drawing
        drawingManager.selectedLayer = nil
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.addPoint(CGPoint(x: 20, y: 20))
        drawingManager.endStroke()
        
        // Should not crash even with nil layer
        XCTAssertEqual(drawingManager.strokes.count, 1, "Should handle nil layer gracefully")
        
        // Test filter mode error handling
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = -1.0 // Invalid intensity
        filterManager.applyFilter(to: nil) // Nil layer
        
        // Should not crash
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Should handle invalid filter application")
        
        // Test transformation mode error handling
        transformationManager.selectedLayer = nil
        transformationManager.moveLayer(by: CGSize(width: 10, height: 10))
        
        // Should not crash
        XCTAssertEqual(transformationManager.transformationOffset, .zero, "Should handle nil layer transformation")
    }
    
    // MARK: - Professional Export Integration Tests
    
    func testProfessionalExportIntegration() {
        // Test that export works with all professional features
        
        // Create comprehensive artwork
        let layer = layerManager.createLayer(name: "Export Layer")
        layerManager.addLayer(layer)
        
        drawingManager.startStroke(at: CGPoint(x: 10, y: 10))
        drawingManager.addPoint(CGPoint(x: 50, y: 50))
        drawingManager.endStroke()
        
        textToolManager.addTextElement(at: CGPoint(x: 30, y: 30))
        
        gradientPatternManager.createGradient(
            startPoint: CGPoint(x: 0, y: 0),
            endPoint: CGPoint(x: 100, y: 100),
            colors: [Color.red, Color.blue]
        )
        
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        // Test export formats
        let formats: [ExportFormat] = [.png, .jpeg, .pdf, .tiff, .bmp, .svg, .psd]
        
        for format in formats {
            exportManager.selectedFormat = format
            exportManager.quality = .high
            
            XCTAssertEqual(exportManager.selectedFormat, format, "Export format should be set")
            XCTAssertEqual(exportManager.quality, .high, "Export quality should be set")
        }
    }
    
    // MARK: - Professional Accessibility Integration Tests
    
    func testProfessionalAccessibilityIntegration() {
        // Test accessibility features across professional modes
        
        // Test drawing mode accessibility
        contentView.currentMode = .drawing
        XCTAssertNotNil(drawingManager.selectedTool, "Drawing tool should be accessible")
        XCTAssertNotNil(drawingManager.brushSettings, "Brush settings should be accessible")
        
        // Test text mode accessibility
        contentView.currentMode = .text
        XCTAssertNotNil(textToolManager.selectedFont, "Text font should be accessible")
        XCTAssertNotNil(textToolManager.textElements, "Text elements should be accessible")
        
        // Test design mode accessibility
        contentView.currentMode = .design
        XCTAssertNotNil(gradientPatternManager.gradients, "Gradients should be accessible")
        XCTAssertNotNil(gradientPatternManager.patterns, "Patterns should be accessible")
        
        // Test layer accessibility
        let layer = layerManager.createLayer(name: "Accessible Layer")
        layerManager.addLayer(layer)
        XCTAssertTrue(layer.isVisible, "Layer should be visible")
        XCTAssertEqual(layer.opacity, 1.0, "Layer should have full opacity")
    }
}

// MARK: - Professional Integration Test Utilities

extension ProfessionalIntegrationTests {
    
    func createProfessionalTestData() -> ProfessionalTestData {
        return ProfessionalTestData(
            layers: createTestLayers(),
            strokes: createTestStrokes(),
            selections: createTestSelections(),
            transformations: createTestTransformations(),
            filters: createTestFilters(),
            textElements: createTestTextElements(),
            gradients: createTestGradients(),
            patterns: createTestPatterns()
        )
    }
    
    private func createTestLayers() -> [DrawingLayer] {
        var layers: [DrawingLayer] = []
        for i in 0..<5 {
            let layer = layerManager.createLayer(name: "Test Layer \(i)")
            layerManager.addLayer(layer)
            layers.append(layer)
        }
        return layers
    }
    
    private func createTestStrokes() -> [DrawingStroke] {
        var strokes: [DrawingStroke] = []
        for i in 0..<10 {
            drawingManager.startStroke(at: CGPoint(x: i * 10, y: i * 10))
            drawingManager.addPoint(CGPoint(x: i * 10 + 20, y: i * 10 + 20))
            drawingManager.endStroke()
            strokes.append(drawingManager.strokes.last!)
        }
        return strokes
    }
    
    private func createTestSelections() -> [Selection] {
        var selections: [Selection] = []
        for i in 0..<3 {
            selectionManager.selectedTool = .rectangle
            selectionManager.startSelection(at: CGPoint(x: i * 20, y: i * 20))
            selectionManager.updateSelection(to: CGPoint(x: i * 20 + 30, y: i * 20 + 30))
            selectionManager.endSelection()
            selections.append(selectionManager.selections.last!)
        }
        return selections
    }
    
    private func createTestTransformations() -> [TransformationData] {
        var transformations: [TransformationData] = []
        for i in 0..<5 {
            let layer = layerManager.layers[i]
            transformationManager.selectedLayer = layer
            transformationManager.moveLayer(by: CGSize(width: i * 5, y: i * 5))
            transformations.append(TransformationData(
                layerId: layer.id,
                offset: CGSize(width: i * 5, y: i * 5),
                angle: Double(i * 10),
                scale: 1.0 + Double(i) * 0.1
            ))
        }
        return transformations
    }
    
    private func createTestFilters() -> [AppliedFilter] {
        var filters: [AppliedFilter] = []
        for i in 0..<3 {
            let layer = layerManager.layers[i]
            filterManager.selectedFilter = .brightness
            filterManager.filterIntensity = 0.5 + Double(i) * 0.1
            filterManager.applyFilter(to: layer)
            filters.append(filterManager.appliedFilters.last!)
        }
        return filters
    }
    
    private func createTestTextElements() -> [TextElement] {
        var textElements: [TextElement] = []
        for i in 0..<2 {
            textToolManager.addTextElement(at: CGPoint(x: i * 50, y: i * 50))
            textElements.append(textToolManager.textElements.last!)
        }
        return textElements
    }
    
    private func createTestGradients() -> [GradientElement] {
        var gradients: [GradientElement] = []
        for i in 0..<2 {
            gradientPatternManager.createGradient(
                startPoint: CGPoint(x: i * 50, y: i * 50),
                endPoint: CGPoint(x: i * 50 + 100, y: i * 50 + 100),
                colors: [Color.red, Color.blue]
            )
            gradients.append(gradientPatternManager.gradients.last!)
        }
        return gradients
    }
    
    private func createTestPatterns() -> [PatternElement] {
        var patterns: [PatternElement] = []
        for i in 0..<2 {
            gradientPatternManager.createPattern(
                center: CGPoint(x: i * 50, y: i * 50),
                size: CGSize(width: 50, height: 50),
                color: Color.green
            )
            patterns.append(gradientPatternManager.patterns.last!)
        }
        return patterns
    }
}

// MARK: - Professional Test Data Structures

struct ProfessionalTestData {
    let layers: [DrawingLayer]
    let strokes: [DrawingStroke]
    let selections: [Selection]
    let transformations: [TransformationData]
    let filters: [AppliedFilter]
    let textElements: [TextElement]
    let gradients: [GradientElement]
    let patterns: [PatternElement]
}

struct TransformationData {
    let layerId: UUID
    let offset: CGSize
    let angle: Double
    let scale: Double
}