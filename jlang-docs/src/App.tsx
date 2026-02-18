import { Routes, Route, Navigate } from 'react-router-dom'
import Layout from './components/Layout'
import Introduction from './pages/Introduction'
import Variables from './pages/Variables'
import Types from './pages/Types'
import ControlFlow from './pages/ControlFlow'
import Functions from './pages/Functions'
import Structs from './pages/Structs'
import NullSafety from './pages/NullSafety'
import ErrorHandling from './pages/ErrorHandling'
import Operators from './pages/Operators'
import Arrays from './pages/Arrays'
import Memory from './pages/Memory'
import GettingStarted from './pages/GettingStarted'

export default function App() {
  return (
    <Routes>
      <Route element={<Layout />}>
        <Route path="/" element={<Introduction />} />
        <Route path="/variables" element={<Variables />} />
        <Route path="/types" element={<Types />} />
        <Route path="/control-flow" element={<ControlFlow />} />
        <Route path="/functions" element={<Functions />} />
        <Route path="/structs" element={<Structs />} />
        <Route path="/null-safety" element={<NullSafety />} />
        <Route path="/error-handling" element={<ErrorHandling />} />
        <Route path="/operators" element={<Operators />} />
        <Route path="/arrays" element={<Arrays />} />
        <Route path="/memory" element={<Memory />} />
        <Route path="/getting-started" element={<GettingStarted />} />
        <Route path="*" element={<Navigate to="/" replace />} />
      </Route>
    </Routes>
  )
}
