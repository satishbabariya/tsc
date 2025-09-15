import SwiftUI

// MARK: - Action Types

enum ColoringAction {
    case colorRegion(regionId: String, color: Color)
    case clearAll
    case clearRegion(regionId: String)
    
    var description: String {
        switch self {
        case .colorRegion(let regionId, let color):
            return "Color \(regionId) with \(color.description)"
        case .clearAll:
            return "Clear all regions"
        case .clearRegion(let regionId):
            return "Clear \(regionId)"
        }
    }
}

// MARK: - Action History

class ActionHistory: ObservableObject {
    @Published private var history: [ColoringAction] = []
    @Published private var currentIndex: Int = -1
    @Published var canUndo: Bool = false
    @Published var canRedo: Bool = false
    
    private let maxHistorySize = 50
    
    func addAction(_ action: ColoringAction) {
        // Remove any actions after current index (when branching from history)
        if currentIndex < history.count - 1 {
            history.removeSubrange((currentIndex + 1)...)
        }
        
        // Add new action
        history.append(action)
        currentIndex = history.count - 1
        
        // Limit history size
        if history.count > maxHistorySize {
            history.removeFirst()
            currentIndex -= 1
        }
        
        updateButtonStates()
    }
    
    func undo() -> ColoringAction? {
        guard canUndo else { return nil }
        
        let action = history[currentIndex]
        currentIndex -= 1
        updateButtonStates()
        
        return action
    }
    
    func redo() -> ColoringAction? {
        guard canRedo else { return nil }
        
        currentIndex += 1
        let action = history[currentIndex]
        updateButtonStates()
        
        return action
    }
    
    func clear() {
        history.removeAll()
        currentIndex = -1
        updateButtonStates()
    }
    
    private func updateButtonStates() {
        canUndo = currentIndex >= 0
        canRedo = currentIndex < history.count - 1
    }
    
    func getHistoryDescription() -> String {
        if history.isEmpty {
            return "No actions yet"
        }
        
        let currentAction = history[currentIndex]
        return "Action \(currentIndex + 1) of \(history.count): \(currentAction.description)"
    }
}

// MARK: - Undo/Redo Manager

class UndoRedoManager: ObservableObject {
    @Published var actionHistory = ActionHistory()
    @Published var coloredRegions: [String: Color] = [:]
    
    func colorRegion(_ regionId: String, with color: Color) {
        let previousColor = coloredRegions[regionId]
        coloredRegions[regionId] = color
        
        // Add to history
        actionHistory.addAction(.colorRegion(regionId: regionId, color: color))
    }
    
    func clearAll() {
        let previousRegions = coloredRegions
        coloredRegions.removeAll()
        
        // Add to history
        actionHistory.addAction(.clearAll)
    }
    
    func clearRegion(_ regionId: String) {
        let previousColor = coloredRegions[regionId]
        coloredRegions[regionId] = nil
        
        // Add to history
        actionHistory.addAction(.clearRegion(regionId: regionId))
    }
    
    func undo() {
        guard let action = actionHistory.undo() else { return }
        
        switch action {
        case .colorRegion(let regionId, _):
            // Remove the color (undo the coloring)
            coloredRegions[regionId] = nil
        case .clearAll:
            // Restore all colors (this would need previous state)
            // For now, just clear all
            coloredRegions.removeAll()
        case .clearRegion(let regionId):
            // Restore the color (this would need previous state)
            // For now, just ensure it's cleared
            coloredRegions[regionId] = nil
        }
    }
    
    func redo() {
        guard let action = actionHistory.redo() else { return }
        
        switch action {
        case .colorRegion(let regionId, let color):
            coloredRegions[regionId] = color
        case .clearAll:
            coloredRegions.removeAll()
        case .clearRegion(let regionId):
            coloredRegions[regionId] = nil
        }
    }
    
    func reset() {
        coloredRegions.removeAll()
        actionHistory.clear()
    }
}

// MARK: - Undo/Redo Buttons View

struct UndoRedoButtonsView: View {
    @ObservedObject var undoRedoManager: UndoRedoManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        HStack(spacing: 15) {
            // Undo Button
            Button(action: {
                withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                    undoRedoManager.undo()
                }
                SafeHapticFeedback.shared.lightImpact()
            }) {
                HStack {
                    Image(systemName: "arrow.uturn.backward")
                    Text("Undo")
                }
                .foregroundColor(.white)
                .padding(.horizontal, 15)
                .padding(.vertical, 8)
                .background(
                    RoundedRectangle(cornerRadius: 15)
                        .fill(undoRedoManager.actionHistory.canUndo ? Color.orange : Color.gray)
                )
            }
            .disabled(!undoRedoManager.actionHistory.canUndo)
            .buttonStyle(PlainButtonStyle())
            
            // Redo Button
            Button(action: {
                withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                    undoRedoManager.redo()
                }
                SafeHapticFeedback.shared.lightImpact()
            }) {
                HStack {
                    Text("Redo")
                    Image(systemName: "arrow.uturn.forward")
                }
                .foregroundColor(.white)
                .padding(.horizontal, 15)
                .padding(.vertical, 8)
                .background(
                    RoundedRectangle(cornerRadius: 15)
                        .fill(undoRedoManager.actionHistory.canRedo ? Color.green : Color.gray)
                )
            }
            .disabled(!undoRedoManager.actionHistory.canRedo)
            .buttonStyle(PlainButtonStyle())
            
            Spacer()
            
            // History Info
            Text(undoRedoManager.actionHistory.getHistoryDescription())
                .font(.caption)
                .foregroundColor(.secondary)
                .padding(.horizontal, 10)
        }
        .padding(.horizontal)
    }
}

// MARK: - Enhanced Coloring Canvas with Undo/Redo

struct EnhancedColoringCanvasView: View {
    let template: ColoringTemplate
    @Binding var selectedColor: Color
    @ObservedObject var errorHandler: ErrorHandler
    @StateObject private var undoRedoManager = UndoRedoManager()
    @State private var showShareSheet = false
    @State private var capturedImage: UIImage?
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Background
                RoundedRectangle(cornerRadius: 20)
                    .fill(Color.white)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Template drawing with colored regions
                    ZStack {
                        // Colored regions (behind the outline)
                        ForEach(template.regions, id: \.id) { region in
                            region.path
                                .fill(undoRedoManager.coloredRegions[region.id] ?? Color.clear)
                                .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                                .onTapGesture {
                                    // Validate region before coloring
                                    guard InputValidator.validateRegionId(region.id) else {
                                        errorHandler.handleError(.invalidRegion)
                                        return
                                    }
                                    
                                    withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                                        undoRedoManager.colorRegion(region.id, with: selectedColor)
                                    }
                                    
                                    // Safe haptic feedback
                                    SafeHapticFeedback.shared.lightImpact()
                                }
                        }
                        
                        // Template outline (on top)
                        template.drawing
                            .stroke(Color.black, lineWidth: 3)
                            .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.white)
                    .cornerRadius(15)
                    .padding()
                    
                    // Undo/Redo buttons
                    UndoRedoButtonsView(undoRedoManager: undoRedoManager, errorHandler: errorHandler)
                    
                    // Action buttons
                    HStack(spacing: 20) {
                        // Clear button
                        Button(action: {
                            withAnimation(.spring()) {
                                undoRedoManager.clearAll()
                            }
                            SafeHapticFeedback.shared.mediumImpact()
                        }) {
                            HStack {
                                Image(systemName: "trash")
                                Text("Clear")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 20)
                            .padding(.vertical, 10)
                            .background(Color.red)
                            .cornerRadius(20)
                        }
                        
                        // Save/Share button
                        Button(action: {
                            guard let image = captureImage() else {
                                errorHandler.handleError(.imageCaptureFailed)
                                return
                            }
                            capturedImage = image
                            showShareSheet = true
                        }) {
                            HStack {
                                Image(systemName: "square.and.arrow.up")
                                Text("Share")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 20)
                            .padding(.vertical, 10)
                            .background(Color.blue)
                            .cornerRadius(20)
                        }
                    }
                    .padding(.bottom, 10)
                }
            }
        }
        .onChange(of: template) { _ in
            withAnimation(.easeInOut) {
                undoRedoManager.reset()
            }
        }
        .sheet(isPresented: $showShareSheet) {
            if let image = capturedImage {
                SafeShareSheet(activityItems: [image], errorHandler: errorHandler)
            }
        }
    }
    
    private func captureImage() -> UIImage? {
        let content = ZStack {
            Color.white
            VStack {
                ZStack {
                    // Colored regions
                    ForEach(template.regions, id: \.id) { region in
                        region.path
                            .fill(undoRedoManager.coloredRegions[region.id] ?? Color.clear)
                    }
                    
                    // Template outline
                    template.drawing
                        .stroke(Color.black, lineWidth: 3)
                }
                .frame(width: 400, height: 400)
            }
        }
        
        return SafeImageCapture.captureImage(from: content, errorHandler: errorHandler)
    }
}

// MARK: - Keyboard Shortcuts Support

struct KeyboardShortcutsView: View {
    @ObservedObject var undoRedoManager: UndoRedoManager
    
    var body: some View {
        VStack {
            // Keyboard shortcuts info
            Text("Keyboard Shortcuts")
                .font(.caption)
                .foregroundColor(.secondary)
            
            HStack {
                Text("⌘Z: Undo")
                    .font(.caption2)
                    .foregroundColor(.secondary)
                
                Text("⌘⇧Z: Redo")
                    .font(.caption2)
                    .foregroundColor(.secondary)
            }
        }
        .onReceive(NotificationCenter.default.publisher(for: .keyboardShortcut)) { notification in
            if let shortcut = notification.object as? String {
                switch shortcut {
                case "undo":
                    undoRedoManager.undo()
                case "redo":
                    undoRedoManager.redo()
                default:
                    break
                }
            }
        }
    }
}

// MARK: - Action History View (for debugging/advanced users)

struct ActionHistoryView: View {
    @ObservedObject var actionHistory: ActionHistory
    
    var body: some View {
        VStack(alignment: .leading, spacing: 5) {
            Text("Action History")
                .font(.headline)
                .foregroundColor(.primary)
            
            ScrollView {
                LazyVStack(alignment: .leading, spacing: 2) {
                    ForEach(Array(actionHistory.history.enumerated()), id: \.offset) { index, action in
                        HStack {
                            Text("\(index + 1).")
                                .font(.caption)
                                .foregroundColor(.secondary)
                            
                            Text(action.description)
                                .font(.caption)
                                .foregroundColor(index <= actionHistory.currentIndex ? .primary : .secondary)
                            
                            Spacer()
                            
                            if index == actionHistory.currentIndex {
                                Text("← Current")
                                    .font(.caption2)
                                    .foregroundColor(.blue)
                            }
                        }
                        .padding(.horizontal, 5)
                    }
                }
            }
            .frame(maxHeight: 200)
        }
        .padding()
        .background(Color.gray.opacity(0.1))
        .cornerRadius(10)
    }
}

// MARK: - Extensions for ActionHistory

extension ActionHistory {
    var history: [ColoringAction] {
        return history
    }
}