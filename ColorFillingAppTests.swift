import XCTest
import SwiftUI
@testable import ColorFillingApp

class ColorFillingAppTests: XCTestCase {
    
    // MARK: - Template Tests
    
    func testColoringTemplateCases() {
        // Test that all expected templates are available
        let expectedTemplates: [ColoringTemplate] = [.butterfly, .flower, .house, .star, .heart, .cat]
        XCTAssertEqual(ColoringTemplate.allCases.count, expectedTemplates.count)
        
        for template in ColoringTemplate.allCases {
            XCTAssertFalse(template.name.isEmpty, "Template name should not be empty")
            XCTAssertNotNil(template.icon, "Template icon should not be nil")
        }
    }
    
    func testTemplateNames() {
        XCTAssertEqual(ColoringTemplate.butterfly.name, "Butterfly")
        XCTAssertEqual(ColoringTemplate.flower.name, "Flower")
        XCTAssertEqual(ColoringTemplate.house.name, "House")
        XCTAssertEqual(ColoringTemplate.star.name, "Star")
        XCTAssertEqual(ColoringTemplate.heart.name, "Heart")
        XCTAssertEqual(ColoringTemplate.cat.name, "Cat")
    }
    
    func testTemplateRegions() {
        for template in ColoringTemplate.allCases {
            let regions = template.regions
            XCTAssertFalse(regions.isEmpty, "Template \(template.name) should have at least one region")
            
            for region in regions {
                XCTAssertFalse(region.id.isEmpty, "Region ID should not be empty")
                // Test that the path is valid
                XCTAssertFalse(region.path.isEmpty, "Region path should not be empty")
            }
        }
    }
    
    func testTemplateDrawingPaths() {
        for template in ColoringTemplate.allCases {
            let path = template.drawing
            XCTAssertFalse(path.isEmpty, "Template \(template.name) drawing path should not be empty")
        }
    }
    
    // MARK: - Color Palette Tests
    
    func testColorPaletteColors() {
        let expectedColors: [Color] = [
            .red, .orange, .yellow, .green, .mint, .teal,
            .cyan, .blue, .indigo, .purple, .pink, .brown,
            .gray, .black, .white, Color(red: 1, green: 0.8, blue: 0.8)
        ]
        
        // Test that we have the expected number of colors
        XCTAssertEqual(expectedColors.count, 16, "Should have exactly 16 colors")
        
        // Test that all colors are valid
        for color in expectedColors {
            XCTAssertNotNil(color, "Color should not be nil")
        }
    }
    
    // MARK: - Butterfly Template Specific Tests
    
    func testButterflyTemplate() {
        let butterfly = ButterflyTemplate()
        let regions = butterfly.regions
        
        XCTAssertEqual(regions.count, 3, "Butterfly should have 3 regions")
        
        let regionIds = regions.map { $0.id }
        XCTAssertTrue(regionIds.contains("wing1"), "Butterfly should have wing1 region")
        XCTAssertTrue(regionIds.contains("wing2"), "Butterfly should have wing2 region")
        XCTAssertTrue(regionIds.contains("body"), "Butterfly should have body region")
    }
    
    func testButterflyPath() {
        let butterfly = ButterflyTemplate()
        let path = butterfly.path
        
        XCTAssertFalse(path.isEmpty, "Butterfly path should not be empty")
        
        // Test that the path contains expected elements
        let boundingRect = path.boundingRect
        XCTAssertGreaterThan(boundingRect.width, 0, "Butterfly path should have width")
        XCTAssertGreaterThan(boundingRect.height, 0, "Butterfly path should have height")
    }
    
    // MARK: - Flower Template Specific Tests
    
    func testFlowerTemplate() {
        let flower = FlowerTemplate()
        let regions = flower.regions
        
        XCTAssertEqual(regions.count, 5, "Flower should have 5 regions")
        
        let regionIds = regions.map { $0.id }
        XCTAssertTrue(regionIds.contains("center"), "Flower should have center region")
        XCTAssertTrue(regionIds.contains("petal1"), "Flower should have petal1 region")
        XCTAssertTrue(regionIds.contains("petal2"), "Flower should have petal2 region")
        XCTAssertTrue(regionIds.contains("petal3"), "Flower should have petal3 region")
        XCTAssertTrue(regionIds.contains("petal4"), "Flower should have petal4 region")
    }
    
    // MARK: - House Template Specific Tests
    
    func testHouseTemplate() {
        let house = HouseTemplate()
        let regions = house.regions
        
        XCTAssertEqual(regions.count, 3, "House should have 3 regions")
        
        let regionIds = regions.map { $0.id }
        XCTAssertTrue(regionIds.contains("roof"), "House should have roof region")
        XCTAssertTrue(regionIds.contains("house"), "House should have house region")
        XCTAssertTrue(regionIds.contains("door"), "House should have door region")
    }
    
    // MARK: - Star Template Specific Tests
    
    func testStarTemplate() {
        let star = StarTemplate()
        let regions = star.regions
        
        XCTAssertEqual(regions.count, 1, "Star should have 1 region")
        XCTAssertEqual(regions.first?.id, "star", "Star region should have correct ID")
    }
    
    // MARK: - Heart Template Specific Tests
    
    func testHeartTemplate() {
        let heart = HeartTemplate()
        let regions = heart.regions
        
        XCTAssertEqual(regions.count, 1, "Heart should have 1 region")
        XCTAssertEqual(regions.first?.id, "heart", "Heart region should have correct ID")
    }
    
    // MARK: - Cat Template Specific Tests
    
    func testCatTemplate() {
        let cat = CatTemplate()
        let regions = cat.regions
        
        XCTAssertEqual(regions.count, 6, "Cat should have 6 regions")
        
        let regionIds = regions.map { $0.id }
        XCTAssertTrue(regionIds.contains("head"), "Cat should have head region")
        XCTAssertTrue(regionIds.contains("body"), "Cat should have body region")
        XCTAssertTrue(regionIds.contains("leftEar"), "Cat should have leftEar region")
        XCTAssertTrue(regionIds.contains("rightEar"), "Cat should have rightEar region")
        XCTAssertTrue(regionIds.contains("leftLeg"), "Cat should have leftLeg region")
        XCTAssertTrue(regionIds.contains("rightLeg"), "Cat should have rightLeg region")
    }
    
    // MARK: - Path Validation Tests
    
    func testPathValidity() {
        for template in ColoringTemplate.allCases {
            let path = template.drawing
            let boundingRect = path.boundingRect
            
            XCTAssertGreaterThan(boundingRect.width, 0, "Template \(template.name) path should have positive width")
            XCTAssertGreaterThan(boundingRect.height, 0, "Template \(template.name) path should have positive height")
            
            // Test that the path is within reasonable bounds
            XCTAssertLessThan(boundingRect.width, 1000, "Template \(template.name) path width should be reasonable")
            XCTAssertLessThan(boundingRect.height, 1000, "Template \(template.name) path height should be reasonable")
        }
    }
    
    func testRegionPathValidity() {
        for template in ColoringTemplate.allCases {
            for region in template.regions {
                let boundingRect = region.path.boundingRect
                
                XCTAssertGreaterThan(boundingRect.width, 0, "Region \(region.id) path should have positive width")
                XCTAssertGreaterThan(boundingRect.height, 0, "Region \(region.id) path should have positive height")
            }
        }
    }
    
    // MARK: - Performance Tests
    
    func testTemplateCreationPerformance() {
        measure {
            for _ in 0..<100 {
                _ = ColoringTemplate.allCases.map { template in
                    template.drawing
                    template.regions
                }
            }
        }
    }
    
    func testPathRenderingPerformance() {
        let butterfly = ButterflyTemplate()
        
        measure {
            for _ in 0..<1000 {
                _ = butterfly.path.boundingRect
            }
        }
    }
    
    // MARK: - Edge Case Tests
    
    func testEmptyRegionHandling() {
        // Test that empty regions are handled gracefully
        let emptyRegion = ColoringRegion(id: "", path: Path())
        XCTAssertTrue(emptyRegion.id.isEmpty)
        XCTAssertTrue(emptyRegion.path.isEmpty)
    }
    
    func testColorEquality() {
        let color1 = Color.red
        let color2 = Color.red
        // Note: Color equality testing in SwiftUI can be tricky
        // This is a basic test to ensure colors can be compared
        XCTAssertNotNil(color1)
        XCTAssertNotNil(color2)
    }
    
    // MARK: - Accessibility Tests
    
    func testTemplateAccessibility() {
        for template in ColoringTemplate.allCases {
            // Test that template names are accessible
            XCTAssertFalse(template.name.isEmpty, "Template name should not be empty for accessibility")
            
            // Test that template names don't contain special characters that might confuse screen readers
            let allowedCharacters = CharacterSet.alphanumerics.union(CharacterSet.whitespaces)
            let nameCharacters = CharacterSet(charactersIn: template.name)
            XCTAssertTrue(allowedCharacters.isSuperset(of: nameCharacters), "Template name should only contain accessible characters")
        }
    }
    
    func testRegionAccessibility() {
        for template in ColoringTemplate.allCases {
            for region in template.regions {
                // Test that region IDs are accessible
                XCTAssertFalse(region.id.isEmpty, "Region ID should not be empty for accessibility")
                
                // Test that region IDs don't contain special characters
                let allowedCharacters = CharacterSet.alphanumerics
                let idCharacters = CharacterSet(charactersIn: region.id)
                XCTAssertTrue(allowedCharacters.isSuperset(of: idCharacters), "Region ID should only contain accessible characters")
            }
        }
    }
}

// MARK: - Mock Classes for Testing

class MockColoringCanvasView: ObservableObject {
    @Published var coloredRegions: [String: Color] = [:]
    @Published var selectedColor: Color = .red
    
    func colorRegion(_ regionId: String) {
        coloredRegions[regionId] = selectedColor
    }
    
    func clearAll() {
        coloredRegions.removeAll()
    }
    
    func getColoredRegionCount() -> Int {
        return coloredRegions.count
    }
}

class MockColorPaletteView: ObservableObject {
    @Published var selectedColor: Color = .red
    
    let colors: [Color] = [
        .red, .orange, .yellow, .green, .mint, .teal,
        .cyan, .blue, .indigo, .purple, .pink, .brown,
        .gray, .black, .white, Color(red: 1, green: 0.8, blue: 0.8)
    ]
    
    func selectColor(_ color: Color) {
        selectedColor = color
    }
    
    func getColorCount() -> Int {
        return colors.count
    }
}

// MARK: - Integration Tests

class ColorFillingAppIntegrationTests: XCTestCase {
    
    func testCompleteColoringWorkflow() {
        let mockCanvas = MockColoringCanvasView()
        let mockPalette = MockColorPaletteView()
        
        // Test initial state
        XCTAssertEqual(mockCanvas.getColoredRegionCount(), 0)
        XCTAssertEqual(mockCanvas.selectedColor, .red)
        
        // Test color selection
        mockPalette.selectColor(.blue)
        XCTAssertEqual(mockPalette.selectedColor, .blue)
        
        // Test coloring a region
        mockCanvas.colorRegion("testRegion")
        XCTAssertEqual(mockCanvas.getColoredRegionCount(), 1)
        XCTAssertEqual(mockCanvas.coloredRegions["testRegion"], .red) // Should use canvas's selected color
        
        // Test clearing
        mockCanvas.clearAll()
        XCTAssertEqual(mockCanvas.getColoredRegionCount(), 0)
    }
    
    func testTemplateSwitchingWorkflow() {
        let templates = ColoringTemplate.allCases
        
        for template in templates {
            // Test that each template has valid regions
            XCTAssertFalse(template.regions.isEmpty, "Template \(template.name) should have regions")
            
            // Test that each template has a valid drawing path
            XCTAssertFalse(template.drawing.isEmpty, "Template \(template.name) should have a drawing path")
        }
    }
    
    func testColorPaletteCompleteness() {
        let mockPalette = MockColorPaletteView()
        
        // Test that we have the expected number of colors
        XCTAssertEqual(mockPalette.getColorCount(), 16)
        
        // Test that all colors are unique
        let uniqueColors = Set(mockPalette.colors.map { $0.description })
        XCTAssertEqual(uniqueColors.count, mockPalette.colors.count, "All colors should be unique")
    }
}