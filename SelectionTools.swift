import SwiftUI
import CoreGraphics

// MARK: - Selection Tool Types

enum SelectionTool: String, CaseIterable {
    case rectangle = "rectangle"
    case ellipse = "ellipse"
    case lasso = "lasso"
    case polygon = "polygon"
    case magicWand = "magic_wand"
    case quickSelection = "quick_selection"
    
    var name: String {
        switch self {
        case .rectangle: return "Rectangle"
        case .ellipse: return "Ellipse"
        case .lasso: return "Lasso"
        case .polygon: return "Polygon"
        case .magicWand: return "Magic Wand"
        case .quickSelection: return "Quick Selection"
        }
    }
    
    var icon: Image {
        switch self {
        case .rectangle: return Image(systemName: "rectangle")
        case .ellipse: return Image(systemName: "oval")
        case .lasso: return Image(systemName: "lasso")
        case .polygon: return Image(systemName: "hexagon")
        case .magicWand: return Image(systemName: "wand.and.stars")
        case .quickSelection: return Image(systemName: "hand.tap")
        }
    }
}

// MARK: - Selection Model

struct Selection: Identifiable {
    let id = UUID()
    let tool: SelectionTool
    let path: Path
    let bounds: CGRect
    let timestamp: Date
    
    init(tool: SelectionTool, path: Path, bounds: CGRect) {
        self.tool = tool
        self.path = path
        self.bounds = bounds
        self.timestamp = Date()
    }
}

// MARK: - Selection Manager

class SelectionManager: ObservableObject {
    @Published var currentTool: SelectionTool = .rectangle
    @Published var selections: [Selection] = []
    @Published var activeSelection: Selection?
    @Published var isSelecting = false
    @Published var selectionPoints: [CGPoint] = []
    @Published var showSelectionPanel = false
    @Published var selectionMode: SelectionMode = .replace
    
    enum SelectionMode: String, CaseIterable {
        case replace = "Replace"
        case add = "Add"
        case subtract = "Subtract"
        case intersect = "Intersect"
        
        var icon: Image {
            switch self {
            case .replace: return Image(systemName: "square")
            case .add: return Image(systemName: "plus.square")
            case .subtract: return Image(systemName: "minus.square")
            case .intersect: return Image(systemName: "square.intersection")
            }
        }
    }
    
    func startSelection(at point: CGPoint) {
        isSelecting = true
        selectionPoints = [point]
        
        if selectionMode == .replace {
            clearSelections()
        }
    }
    
    func addSelectionPoint(_ point: CGPoint) {
        guard isSelecting else { return }
        selectionPoints.append(point)
    }
    
    func endSelection() {
        guard isSelecting, selectionPoints.count >= 2 else {
            isSelecting = false
            selectionPoints = []
            return
        }
        
        let selection = createSelection()
        if let selection = selection {
            applySelectionMode(selection)
        }
        
        isSelecting = false
        selectionPoints = []
    }
    
    private func createSelection() -> Selection? {
        let path: Path
        let bounds: CGRect
        
        switch currentTool {
        case .rectangle:
            guard selectionPoints.count >= 2 else { return nil }
            let startPoint = selectionPoints[0]
            let endPoint = selectionPoints[1]
            bounds = CGRect(
                x: min(startPoint.x, endPoint.x),
                y: min(startPoint.y, endPoint.y),
                width: abs(endPoint.x - startPoint.x),
                height: abs(endPoint.y - startPoint.y)
            )
            path = Path(bounds)
            
        case .ellipse:
            guard selectionPoints.count >= 2 else { return nil }
            let startPoint = selectionPoints[0]
            let endPoint = selectionPoints[1]
            bounds = CGRect(
                x: min(startPoint.x, endPoint.x),
                y: min(startPoint.y, endPoint.y),
                width: abs(endPoint.x - startPoint.x),
                height: abs(endPoint.y - startPoint.y)
            )
            path = Path(ellipseIn: bounds)
            
        case .lasso:
            guard selectionPoints.count >= 3 else { return nil }
            var pathBuilder = Path()
            pathBuilder.move(to: selectionPoints[0])
            for i in 1..<selectionPoints.count {
                pathBuilder.addLine(to: selectionPoints[i])
            }
            pathBuilder.closeSubpath()
            path = pathBuilder
            bounds = path.boundingRect
            
        case .polygon:
            guard selectionPoints.count >= 3 else { return nil }
            var pathBuilder = Path()
            pathBuilder.move(to: selectionPoints[0])
            for i in 1..<selectionPoints.count {
                pathBuilder.addLine(to: selectionPoints[i])
            }
            pathBuilder.closeSubpath()
            path = pathBuilder
            bounds = path.boundingRect
            
        case .magicWand:
            // Simplified magic wand - select similar colors in a region
            guard let point = selectionPoints.first else { return nil }
            let region = CGRect(x: point.x - 10, y: point.y - 10, width: 20, height: 20)
            path = Path(ellipseIn: region)
            bounds = region
            
        case .quickSelection:
            // Simplified quick selection - expand from initial point
            guard let point = selectionPoints.first else { return nil }
            let region = CGRect(x: point.x - 15, y: point.y - 15, width: 30, height: 30)
            path = Path(ellipseIn: region)
            bounds = region
        }
        
        return Selection(tool: currentTool, path: path, bounds: bounds)
    }
    
    private func applySelectionMode(_ selection: Selection) {
        switch selectionMode {
        case .replace:
            selections = [selection]
            activeSelection = selection
            
        case .add:
            selections.append(selection)
            activeSelection = selection
            
        case .subtract:
            // Remove overlapping areas
            selections = selections.filter { !selection.path.intersects($0.path) }
            if !selections.isEmpty {
                activeSelection = selections.last
            }
            
        case .intersect:
            // Keep only intersecting areas
            selections = selections.filter { selection.path.intersects($0.path) }
            if !selections.isEmpty {
                activeSelection = selections.last
            }
        }
    }
    
    func clearSelections() {
        selections.removeAll()
        activeSelection = nil
    }
    
    func deleteSelection(_ selection: Selection) {
        selections.removeAll { $0.id == selection.id }
        if activeSelection?.id == selection.id {
            activeSelection = selections.last
        }
    }
    
    func selectAll() {
        let fullBounds = CGRect(x: 0, y: 0, width: 400, height: 400)
        let fullSelection = Selection(
            tool: .rectangle,
            path: Path(fullBounds),
            bounds: fullBounds
        )
        selections = [fullSelection]
        activeSelection = fullSelection
    }
    
    func invertSelection() {
        // Simplified inversion - select everything except current selections
        let fullBounds = CGRect(x: 0, y: 0, width: 400, height: 400)
        let fullSelection = Selection(
            tool: .rectangle,
            path: Path(fullBounds),
            bounds: fullBounds
        )
        selections = [fullSelection]
        activeSelection = fullSelection
    }
}

// MARK: - Selection Tool Palette

struct SelectionToolPalette: View {
    @ObservedObject var selectionManager: SelectionManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("✂️ Selection Tools")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        selectionManager.showSelectionPanel.toggle()
                    }
                }) {
                    Image(systemName: selectionManager.showSelectionPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if selectionManager.showSelectionPanel {
                VStack(spacing: 15) {
                    // Tool selector
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(SelectionTool.allCases, id: \.self) { tool in
                                Button(action: {
                                    selectionManager.currentTool = tool
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    VStack {
                                        tool.icon
                                            .font(.system(size: 25))
                                            .foregroundColor(selectionManager.currentTool == tool ? .white : .primary)
                                            .frame(width: 50, height: 50)
                                            .background(
                                                Circle()
                                                    .fill(selectionManager.currentTool == tool ? Color.blue : Color.gray.opacity(0.2))
                                            )
                                        
                                        Text(tool.name)
                                            .font(.caption)
                                            .foregroundColor(.primary)
                                    }
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        .padding(.horizontal)
                    }
                    
                    // Selection mode
                    VStack {
                        Text("Selection Mode")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        HStack(spacing: 10) {
                            ForEach(SelectionManager.SelectionMode.allCases, id: \.self) { mode in
                                Button(action: {
                                    selectionManager.selectionMode = mode
                                }) {
                                    VStack {
                                        mode.icon
                                            .font(.system(size: 20))
                                            .foregroundColor(selectionManager.selectionMode == mode ? .white : .primary)
                                        
                                        Text(mode.rawValue)
                                            .font(.caption2)
                                            .foregroundColor(selectionManager.selectionMode == mode ? .white : .primary)
                                    }
                                    .padding(.horizontal, 8)
                                    .padding(.vertical, 5)
                                    .background(
                                        RoundedRectangle(cornerRadius: 8)
                                            .fill(selectionManager.selectionMode == mode ? Color.blue : Color.gray.opacity(0.2))
                                    )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                    }
                    
                    // Selection actions
                    HStack(spacing: 15) {
                        Button("Select All") {
                            selectionManager.selectAll()
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        .buttonStyle(.bordered)
                        
                        Button("Invert") {
                            selectionManager.invertSelection()
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        .buttonStyle(.bordered)
                        
                        Button("Clear") {
                            selectionManager.clearSelections()
                            SafeHapticFeedback.shared.clear()
                        }
                        .buttonStyle(.bordered)
                    }
                }
                .padding()
                .background(
                    RoundedRectangle(cornerRadius: 15)
                        .fill(Color.gray.opacity(0.1))
                )
                .padding(.horizontal)
            }
        }
        .padding(.vertical, 10)
        .background(
            LinearGradient(
                colors: [Color.orange.opacity(0.05), Color.red.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Selection View

struct SelectionView: View {
    let selection: Selection
    @ObservedObject var selectionManager: SelectionManager
    @State private var isDragging = false
    @State private var dragOffset: CGSize = .zero
    
    var body: some View {
        selection.path
            .stroke(Color.blue, lineWidth: 2)
            .background(
                selection.path
                    .fill(Color.blue.opacity(0.1))
            )
            .overlay(
                // Selection handles
                SelectionHandlesView(selection: selection, selectionManager: selectionManager)
            )
            .gesture(
                DragGesture()
                    .onChanged { value in
                        if !isDragging {
                            isDragging = true
                        }
                        dragOffset = value.translation
                    }
                    .onEnded { value in
                        // Move selection
                        moveSelection(by: value.translation)
                        isDragging = false
                        dragOffset = .zero
                    }
            )
            .onTapGesture {
                selectionManager.activeSelection = selection
            }
    }
    
    private func moveSelection(by offset: CGSize) {
        // Move selection logic would be implemented here
        // This would involve updating the selection's path and bounds
    }
}

// MARK: - Selection Handles View

struct SelectionHandlesView: View {
    let selection: Selection
    @ObservedObject var selectionManager: SelectionManager
    @State private var isResizing = false
    @State private var resizeHandle: ResizeHandle?
    
    enum ResizeHandle: CaseIterable {
        case topLeft, topRight, bottomLeft, bottomRight
        case top, bottom, left, right
        
        var position: CGPoint {
            switch self {
            case .topLeft: return CGPoint(x: 0, y: 0)
            case .topRight: return CGPoint(x: 1, y: 0)
            case .bottomLeft: return CGPoint(x: 0, y: 1)
            case .bottomRight: return CGPoint(x: 1, y: 1)
            case .top: return CGPoint(x: 0.5, y: 0)
            case .bottom: return CGPoint(x: 0.5, y: 1)
            case .left: return CGPoint(x: 0, y: 0.5)
            case .right: return CGPoint(x: 1, y: 0.5)
            }
        }
    }
    
    var body: some View {
        ForEach(ResizeHandle.allCases, id: \.self) { handle in
            Circle()
                .fill(Color.blue)
                .frame(width: 8, height: 8)
                .position(
                    x: selection.bounds.minX + selection.bounds.width * handle.position.x,
                    y: selection.bounds.minY + selection.bounds.height * handle.position.y
                )
                .gesture(
                    DragGesture()
                        .onChanged { value in
                            if !isResizing {
                                isResizing = true
                                resizeHandle = handle
                            }
                            resizeSelection(handle: handle, translation: value.translation)
                        }
                        .onEnded { _ in
                            isResizing = false
                            resizeHandle = nil
                        }
                )
        }
    }
    
    private func resizeSelection(handle: ResizeHandle, translation: CGSize) {
        // Resize selection logic would be implemented here
        // This would involve updating the selection's bounds and path
    }
}

// MARK: - Selection Canvas View

struct SelectionCanvasView: View {
    @ObservedObject var selectionManager: SelectionManager
    @ObservedObject var errorHandler: ErrorHandler
    @State private var canvasSize: CGSize = CGSize(width: 400, height: 400)
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Canvas background
                RoundedRectangle(cornerRadius: 20)
                    .fill(Color.white)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Canvas area
                    ZStack {
                        // Canvas
                        Rectangle()
                            .fill(Color.white)
                            .frame(width: canvasSize.width, height: canvasSize.height)
                            .border(Color.gray.opacity(0.3), width: 1)
                        
                        // Selections
                        ForEach(selectionManager.selections) { selection in
                            SelectionView(selection: selection, selectionManager: selectionManager)
                        }
                        
                        // Current selection being drawn
                        if selectionManager.isSelecting && !selectionManager.selectionPoints.isEmpty {
                            CurrentSelectionView(selectionManager: selectionManager)
                        }
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.white)
                    .cornerRadius(15)
                    .padding()
                    .gesture(
                        DragGesture(minimumDistance: 0)
                            .onChanged { value in
                                let point = value.location
                                
                                if !selectionManager.isSelecting {
                                    selectionManager.startSelection(at: point)
                                } else {
                                    selectionManager.addSelectionPoint(point)
                                }
                            }
                            .onEnded { _ in
                                selectionManager.endSelection()
                            }
                    )
                    
                    // Selection info
                    HStack {
                        Text("Selections: \(selectionManager.selections.count)")
                            .font(.caption)
                            .foregroundColor(.secondary)
                        
                        Spacer()
                        
                        if let activeSelection = selectionManager.activeSelection {
                            Text("Active: \(activeSelection.tool.name)")
                                .font(.caption)
                                .foregroundColor(.blue)
                        }
                    }
                    .padding(.horizontal)
                    .padding(.bottom, 10)
                }
            }
        }
    }
}

// MARK: - Current Selection View

struct CurrentSelectionView: View {
    @ObservedObject var selectionManager: SelectionManager
    
    var body: some View {
        if selectionManager.selectionPoints.count >= 2 {
            switch selectionManager.currentTool {
            case .rectangle:
                RectangleSelectionPreview(points: selectionManager.selectionPoints)
            case .ellipse:
                EllipseSelectionPreview(points: selectionManager.selectionPoints)
            case .lasso, .polygon:
                LassoSelectionPreview(points: selectionManager.selectionPoints)
            case .magicWand, .quickSelection:
                CircleSelectionPreview(points: selectionManager.selectionPoints)
            }
        }
    }
}

// MARK: - Selection Previews

struct RectangleSelectionPreview: View {
    let points: [CGPoint]
    
    var body: some View {
        let startPoint = points[0]
        let endPoint = points[1]
        let rect = CGRect(
            x: min(startPoint.x, endPoint.x),
            y: min(startPoint.y, endPoint.y),
            width: abs(endPoint.x - startPoint.x),
            height: abs(endPoint.y - startPoint.y)
        )
        
        return Path(rect)
            .stroke(Color.blue, lineWidth: 2, dash: [5, 5])
            .background(
                Path(rect)
                    .fill(Color.blue.opacity(0.1))
            )
    }
}

struct EllipseSelectionPreview: View {
    let points: [CGPoint]
    
    var body: some View {
        let startPoint = points[0]
        let endPoint = points[1]
        let rect = CGRect(
            x: min(startPoint.x, endPoint.x),
            y: min(startPoint.y, endPoint.y),
            width: abs(endPoint.x - startPoint.x),
            height: abs(endPoint.y - startPoint.y)
        )
        
        return Path(ellipseIn: rect)
            .stroke(Color.blue, lineWidth: 2, dash: [5, 5])
            .background(
                Path(ellipseIn: rect)
                    .fill(Color.blue.opacity(0.1))
            )
    }
}

struct LassoSelectionPreview: View {
    let points: [CGPoint]
    
    var body: some View {
        var path = Path()
        guard !points.isEmpty else { return path }
        
        path.move(to: points[0])
        for i in 1..<points.count {
            path.addLine(to: points[i])
        }
        
        return path
            .stroke(Color.blue, lineWidth: 2, dash: [5, 5])
            .background(
                path
                    .fill(Color.blue.opacity(0.1))
            )
    }
}

struct CircleSelectionPreview: View {
    let points: [CGPoint]
    
    var body: some View {
        guard let point = points.first else { return AnyView(EmptyView()) }
        let radius: CGFloat = 15
        
        return AnyView(
            Circle()
                .stroke(Color.blue, lineWidth: 2, dash: [5, 5])
                .background(
                    Circle()
                        .fill(Color.blue.opacity(0.1))
                )
                .frame(width: radius * 2, height: radius * 2)
                .position(point)
        )
    }
}

// MARK: - Selection Statistics

extension SelectionManager {
    var totalSelections: Int {
        return selections.count
    }
    
    var mostUsedTool: SelectionTool {
        let toolCounts = Dictionary(grouping: selections, by: { $0.tool })
            .mapValues { $0.count }
        
        return toolCounts.max(by: { $0.value < $1.value })?.key ?? .rectangle
    }
    
    var totalSelectionArea: Double {
        return selections.reduce(0) { total, selection in
            total + Double(selection.bounds.width * selection.bounds.height)
        }
    }
}